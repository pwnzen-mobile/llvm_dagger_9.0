//===-- lib/DC/DCTranslator.cpp - DC Translation Engine ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/DC/DCTranslator.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/DC/DCInstrSema.h"
#include "llvm/DC/DCRegisterSema.h"
#include "llvm/MC/MCAnalysis/MCFunction.h"
#include "llvm/MC/MCAnalysis/MCModule.h"
#include "llvm/MC/MCObjectDisassembler.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/IR/Value.h"
#include <algorithm>
#include <vector>
#include <set>
#include <sstream>

namespace llvm {
class NonVolatileRegistersPass : public FunctionPass {
    public:
        static char ID;

        NonVolatileRegistersPass() : FunctionPass(ID) { };

        virtual bool runOnFunction(Function &F);

    private:
        bool hasCall(const BasicBlock &BB);
        bool isNonVolatile(uint64_t Idx);
        bool isStack(uint64_t Idx);
        void replaceLoaded(Instruction *StoreInst);
    };
}

using namespace llvm;

char NonVolatileRegistersPass::ID = 0;

bool NonVolatileRegistersPass::runOnFunction(Function &F) {

        if (F.isDeclaration() || F.isIntrinsic()) {
            return false;
        }
        std::set<Value *> NonVolatileAndStackPointers;
        std::set<Value *> NonVolatilePointers;

        BasicBlock &Entry = F.getEntryBlock();
        for (BasicBlock::iterator I_it = Entry.begin(); I_it != Entry.end(); ++I_it) {
            if (I_it->getOpcode() == Instruction::GetElementPtr) {
                if (ConstantInt *ConstIdx = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
                    if (isNonVolatile(ConstIdx->getZExtValue())) {
                        NonVolatileAndStackPointers.insert(&*I_it);
                        NonVolatilePointers.insert(&*I_it);
                    } else if (isStack(ConstIdx->getZExtValue())) {
                        NonVolatileAndStackPointers.insert(&*I_it);
                    }
                }
            }
        }

        for (Function::iterator BB_it = F.begin(); BB_it != F.end(); ++BB_it) {
            if (!hasCall(*BB_it))
                continue;
            for (BasicBlock::iterator I_it = BB_it->begin(); I_it != BB_it->end(); ++I_it) {
                if (I_it->getOpcode() == Instruction::Store) {
                    std::set<Value *>::iterator S_it = NonVolatileAndStackPointers.find(I_it->getOperand(1));
                    if (S_it != NonVolatileAndStackPointers.end()) {
                        replaceLoaded(&*I_it);
                    }
                }
            }
        }

        std::set<Instruction*> toRemove;

        for (auto &ptr : NonVolatilePointers) {

            for (auto ptrUser : ptr->users()) {
                if (StoreInst *storeInst = dyn_cast<StoreInst>(ptrUser)) {
                    toRemove.insert(storeInst);
                }
            }
        }

        for (auto &r : toRemove) {
            r->dropAllReferences();
            r->removeFromParent();
//                r->dump();
        }
        return true;
}

bool NonVolatileRegistersPass::hasCall(const BasicBlock &BB) {
    for (BasicBlock::const_iterator I_it = BB.begin(); I_it != BB.end(); ++I_it) {
        if (I_it->getOpcode() == Instruction::Call) {
            return true;
        }
    }
    return false;
}

bool NonVolatileRegistersPass::isNonVolatile(uint64_t Idx) {
    switch (Idx) {
        default:
            break;
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
            return true;
    }
    return false;
}

bool NonVolatileRegistersPass::isStack(uint64_t Idx) {
    switch (Idx) {
        default:
            break;
        case 3:
        case 0:
            return true;
    }
    return false;
}

void NonVolatileRegistersPass::replaceLoaded(Instruction *StoreInst) {
    Value *Ptr = StoreInst->getOperand(1);
    Value *V = StoreInst->getOperand(0);
   // if (!V)
   //     StoreInst->dump();
    assert(V);

    BasicBlock *BB = StoreInst->getParent();
    Instruction *Load = nullptr;
    for (BasicBlock::iterator I_it = BB->begin(); I_it != BB->end(); ++I_it) {
        if (I_it->getOpcode() == Instruction::Load && I_it->getOperand(0) == Ptr) {
            Load = &*I_it;
        }
    }

    if (!Load)
        return;

//    for (Value::const_use_iterator U_it = Load->use_begin(); U_it != Load->use_end(); ++U_it) {
//
//    }
    if (Load != V)
        Load->replaceAllUsesWith(V);
}

#define DEBUG_TYPE "dctranslator"

DCTranslator::DCTranslator(LLVMContext &Ctx, const DataLayout &DL,
                           TransOpt::Level TransOptLevel, DCInstrSema &DIS,
                           DCRegisterSema &DRS, MCInstPrinter &IP,
                           const MCSubtargetInfo &STI, MCModule &MCM,
                           MCObjectDisassembler *MCOD, bool EnableIRAnnotation)
    : Ctx(Ctx), DL(DL), ModuleSet(), MCOD(MCOD), MCM(MCM),
      CurrentModule(nullptr), CurrentFPM(), DTIT(), AnnotWriter(), DIS(DIS),
      OptLevel(TransOptLevel) {

  // FIXME: now this can move to print, we don't need to keep it around
  if (EnableIRAnnotation)
    AnnotWriter.reset(new DCAnnotationWriter(DTIT, DRS.MRI, IP, STI));

  finalizeTranslationModule();
}

Module *DCTranslator::finalizeTranslationModule() {
  Module *OldModule = CurrentModule;

  ModuleSet.emplace_back(
      CurrentModule = new Module(
          (Twine("dct module #") + utohexstr(ModuleSet.size())).str(), Ctx));
  CurrentModule->setDataLayout(DL);

  CurrentFPM.reset(new legacy::FunctionPassManager(CurrentModule));

  if (OptLevel >= TransOpt::Less) {
    CurrentFPM->add(new NonVolatileRegistersPass());
    CurrentFPM->add(createInstructionCombiningPass());
    CurrentFPM->add(createSROAPass());
//    CurrentFPM->add(createCFGSimplificationPass());
//    CurrentFPM->add(createConstantPropagationPass());

//    CurrentFPM->add(createPromoteMemoryToRegisterPass());
  }
  if (OptLevel >= TransOpt::Default)
    CurrentFPM->add(createDeadCodeEliminationPass());
  if (OptLevel >= TransOpt::Aggressive)
    CurrentFPM->add(createInstructionCombiningPass());

  DIS.SwitchToModule(CurrentModule);
  return OldModule;
}

void DCTranslator::translateAllKnownFunctions() {
  MCObjectDisassembler::AddressSetTy DummyTailCallTargets;
  for (const auto &F : MCM.funcs()) {
//      if (F->getName() != "fn_100ADE014")
//          continue;
    //  StringRef FAddress = F->getName().substr(3);
    //  uint64_t address = 0;
    //  FAddress.getAsInteger<uint64_t>(16, address);
    //  if (address && address < 0x100BC2AE4) {
    //      continue;
    //  }
      translateFunction(&*F, DummyTailCallTargets);
  }
}

DCTranslator::~DCTranslator() {}

Function *DCTranslator::getInitRegSetFunction() {
  return DIS.getOrCreateInitRegSetFunction();
}
Function *DCTranslator::getFiniRegSetFunction() {
  return DIS.getOrCreateFiniRegSetFunction();
}
Function *DCTranslator::createMainFunctionWrapper(Function *Entrypoint) {
  return DIS.getOrCreateMainFunction(Entrypoint);
}

Function *DCTranslator::translateRecursivelyAt(uint64_t Addr) {
  SmallSetVector<uint64_t, 16> WorkList;
  WorkList.insert(Addr);
  for (size_t i = 0; i < WorkList.size(); ++i) {
    uint64_t Addr = WorkList[i];
    // FIXME: look up in other modules
    Function *F = CurrentModule->getFunction("fn_" + utohexstr(Addr));
    if (F && !F->isDeclaration())
      continue;

    DEBUG(dbgs() << "Translating function at " << utohexstr(Addr) << "\n");

    if (!MCOD) {
      llvm_unreachable(("Unable to translate unknown function at " +
                        utohexstr(Addr) + " without a disassembler!").c_str());
    }

    MCObjectDisassembler::AddressSetTy CallTargets, TailCallTargets;
    MCFunction *MCFN = MCOD->createFunction(&MCM, Addr, CallTargets, TailCallTargets);

    // If the function is empty, it is the declaration of an external function.
    if (MCFN->empty()) {
      StringRef ExtFnName = MCFN->getName();
      assert(!ExtFnName.empty() && "Unnamed function declaration!");
      DEBUG(dbgs() << "Found external function: " << ExtFnName << "\n");
      DIS.createExternalWrapperFunction(Addr, ExtFnName);
      continue;
    }

    translateFunction(MCFN, TailCallTargets);
    for (auto CallTarget : CallTargets)
      WorkList.insert(CallTarget);
  }
  return CurrentModule->getFunction("fn_" + utohexstr(Addr));
}

namespace {
class AddrPrettyStackTraceEntry : public PrettyStackTraceEntry {
public:
  uint64_t StartAddr;
  const char *Kind;
  AddrPrettyStackTraceEntry(uint64_t StartAddr, const char *Kind)
      : PrettyStackTraceEntry(), StartAddr(StartAddr), Kind(Kind) {}

  void print(raw_ostream &OS) const override {
    OS << "DC: Translating " << Kind << " at address " << utohexstr(StartAddr)
       << "\n";
  }
};
} // end anonymous namespace

static bool BBBeginAddrLess(const MCBasicBlock *LHS, const MCBasicBlock *RHS) {
  return LHS->getStartAddr() < RHS->getStartAddr();
}

void DCTranslator::translateFunction(
    MCFunction *MCFN,
    const MCObjectDisassembler::AddressSetTy &TailCallTargets) {

  AddrPrettyStackTraceEntry X(MCFN->getEntryBlock()->getStartAddr(),
                              "Function");

  DIS.SwitchToFunction(MCFN);

  // First, make sure all basic blocks are created, and sorted.
  std::vector<const MCBasicBlock *> BasicBlocks;
  std::copy(MCFN->begin(), MCFN->end(), std::back_inserter(BasicBlocks));
  std::sort(BasicBlocks.begin(), BasicBlocks.end(), BBBeginAddrLess);
  for (auto &BB : BasicBlocks)
    DIS.getOrCreateBasicBlock(BB->getStartAddr());

  for (auto &BB : *MCFN) {
    AddrPrettyStackTraceEntry X(BB->getStartAddr(), "Basic Block");

    DEBUG(dbgs() << "Translating basic block starting at 0x"
                 << utohexstr(BB->getStartAddr()) << ", with " << BB->size()
                 << " instructions.\n");
    DIS.SwitchToBasicBlock(BB);
    for (auto &I : *BB) {
      //(dbgs() << "Translating instruction:\n " << I.Inst << " at 0x" << utohexstr(I.Address) << "\n");
      DCTranslatedInst TI(I);
      if (!DIS.translateInst(I, TI)) {
        errs() << "Cannot translate instruction: \n  ";
        errs() << I.Inst << "\n";
        // llvm_unreachable("Couldn't translate instruction\n");
      }
      if (AnnotWriter)
        DTIT.trackInst(TI);
    }
    DIS.FinalizeBasicBlock();
  }

  for (auto TailCallTarget : TailCallTargets)
    DIS.createExternalTailCallBB(TailCallTarget);

  Function *Fn = DIS.FinalizeFunction();
  {
    // ValueToValueMapTy VMap;
    // Function *OrigFn = CloneFunction(Fn, VMap, false);
    // OrigFn->setName(Fn->getName() + "_orig");
    // CurrentModule->getFunctionList().push_back(OrigFn);
    CurrentFPM->run(*Fn);
  }
}

void DCTranslator::printCurrentModule(raw_ostream &OS) {
  CurrentModule->print(OS, AnnotWriter.get());
}
