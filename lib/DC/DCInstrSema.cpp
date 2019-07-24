//===-- lib/DC/DCInstrSema.cpp - DC Instruction Semantics -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/DC/DCInstrSema.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/DC/DCRegisterSema.h"
#include "llvm/DC/DCTranslatedInstTracker.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"  
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/MC/MCAnalysis/MCFunction.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "dc-sema"

static cl::opt<bool> EnableRegSetDiff("enable-dc-regset-diff", cl::desc(""),
                                      cl::init(false));

static cl::opt<bool> EnableInstAddrSave("enable-dc-pc-save", cl::desc(""),
                                        cl::init(false));

DCInstrSema::DCInstrSema(const unsigned *OpcodeToSemaIdx,
                         const unsigned *SemanticsArray,
                         const uint64_t *ConstantArray, DCRegisterSema &DRS)
    : OpcodeToSemaIdx(OpcodeToSemaIdx), SemanticsArray(SemanticsArray),
      ConstantArray(ConstantArray), DynTranslateAtCBPtr(0), Ctx(0),
      TheModule(0), DRS(DRS), FuncType(0), TheFunction(0), TheMCFunction(0),
      BBByAddr(), ExitBB(0), CallBBs(), TheBB(0), TheMCBB(0), Builder(), Idx(0),
      ResEVT(), Opcode(0), Vals(), CurrentInst(0) {}

DCInstrSema::~DCInstrSema() {}

Function *DCInstrSema::FinalizeFunction() {
  for (auto *CallBB : CallBBs) {
    assert(CallBB->size() == 2 &&
           "Call basic block has wrong number of instructions!");
    auto CallI = CallBB->begin();
    DRS.saveAllLocalRegs(CallBB, CallI);
    DRS.restoreLocalRegs(CallBB, ++CallI);
  }
  DRS.FinalizeFunction(ExitBB);
  CallBBs.clear();
  BBByAddr.clear();
  Function *Fn = TheFunction;
  TheFunction = nullptr;
  TheMCFunction = nullptr;
  return Fn;
}

void DCInstrSema::FinalizeBasicBlock() {
  if (!TheBB->getTerminator())
    BranchInst::Create(getOrCreateBasicBlock(getBasicBlockEndAddress()), TheBB);
  DRS.FinalizeBasicBlock();
  TheBB = nullptr;
  TheMCBB = nullptr;
}

Function *DCInstrSema::getOrCreateMainFunction(Function *EntryFn) {
  Type *MainArgs[] = {Builder->getInt32Ty(),
                      Builder->getInt8PtrTy()->getPointerTo()};
  Function *IRMain = dyn_cast<Function>(TheModule->getOrInsertFunction(
      "main", FunctionType::get(Builder->getInt32Ty(), MainArgs, false)).getCallee());

  if (!IRMain->empty())
    return IRMain;

  IRBuilderBase::InsertPointGuard IPG(*Builder);
  Builder->SetInsertPoint(BasicBlock::Create(*Ctx, "", IRMain));

  AllocaInst *Regset = Builder->CreateAlloca(DRS.getRegSetType());

  // allocate a small local array to serve as a test stack.
  AllocaInst *Stack =
      Builder->CreateAlloca(ArrayType::get(Builder->getInt8Ty(), 8192));

  // 64byte alignment ought to be enough for anybody.
  // FIXME: this should probably be the maximum natural alignment of the
  // non-int-coerced register types. As it is, large integer types are only
  // aligned to the size of the largest legal integer, which isn't enough.
  Regset->setAlignment(64);
  Stack->setAlignment(64);

  Value *StackSize = Builder->getInt32(8192);
  Value *Idx[2] = {Builder->getInt32(0), Builder->getInt32(0)};
  Value *StackPtr = Builder->CreateInBoundsGEP(Stack, Idx);

  //Function::arg_iterator ArgI = IRMain->getArgumentList().begin();
  Function::arg_iterator ArgI = IRMain->args().begin();
  Value *ArgC = ArgI++;
  Value *ArgV = ArgI++;

  Function *InitFn = getOrCreateInitRegSetFunction();
  Function *FiniFn = getOrCreateFiniRegSetFunction();

  Builder->CreateCall(InitFn, {Regset, StackPtr, StackSize, ArgC, ArgV});
  Builder->CreateCall(EntryFn, {Regset});
  Builder->CreateRet(Builder->CreateCall(FiniFn, {Regset}));
  return IRMain;
}

Function *DCInstrSema::getOrCreateInitRegSetFunction() {
  StructType *RegSetType = DRS.getRegSetType();

  Type *InitArgs[] = {RegSetType->getPointerTo(), Builder->getInt8PtrTy(),
                      Builder->getInt32Ty(), Builder->getInt32Ty(),
                      Builder->getInt8PtrTy()->getPointerTo()};
  Function *InitFn = dyn_cast<Function>(TheModule->getOrInsertFunction(
      "main_init_regset",
      FunctionType::get(Builder->getVoidTy(), InitArgs, false)).getCallee());
  if (InitFn->empty())
    DRS.insertInitRegSetCode(InitFn);

  // If we need to diff regsets, now's a good time to insert the function.
  // FIXME: bad hijack
  if (EnableRegSetDiff)
    DRS.getOrCreateRegSetDiffFunction(/*Definition=*/true);

  return InitFn;
}

Function *DCInstrSema::getOrCreateFiniRegSetFunction() {
  StructType *RegSetType = DRS.getRegSetType();

  Type *FiniArgs[] = {RegSetType->getPointerTo()};
  Function *FiniFn = dyn_cast<Function>(TheModule->getOrInsertFunction(
      "main_fini_regset",
      FunctionType::get(Builder->getInt32Ty(), FiniArgs, false)).getCallee());

  if (FiniFn->empty())
    DRS.insertFiniRegSetCode(FiniFn);
  return FiniFn;
}

void DCInstrSema::createExternalWrapperFunction(uint64_t Addr, StringRef Name) {
  Function *ExtFn = dyn_cast<Function>(TheModule->getOrInsertFunction(
      Name, FunctionType::get(Builder->getVoidTy(), false)).getCallee());

  Function *Fn = getFunction(Addr);
  if (!Fn->isDeclaration())
    return;

  BasicBlock *BB = BasicBlock::Create(*Ctx, "", Fn);
  DRS.insertExternalWrapperAsm(BB, ExtFn);
  ReturnInst::Create(*Ctx, BB);
}

void DCInstrSema::createExternalTailCallBB(uint64_t Addr) {
  // First create a basic block for the tail call.
  SwitchToBasicBlock(Addr);
  // Now do the call to that function.
  insertCallBB(getFunction(Addr));
  // Finally, return directly, bypassing the ExitBB.
  Builder->CreateRetVoid();
}

void DCInstrSema::SwitchToModule(Module *M) {
  TheModule = M;
  Ctx = &TheModule->getContext();
  DRS.SwitchToModule(TheModule);
  FuncType = FunctionType::get(Type::getVoidTy(*Ctx),
                               DRS.getRegSetType()->getPointerTo(), false);
  Builder.reset(new DCIRBuilder(*Ctx));
}

extern "C" uintptr_t __llvm_dc_current_fn = 0;
extern "C" uintptr_t __llvm_dc_current_bb = 0;
extern "C" uintptr_t __llvm_dc_current_instr = 0;

void DCInstrSema::SwitchToFunction(const MCFunction *MCFN) {
  assert(!MCFN->empty() && "Trying to translate empty MC function");
  const uint64_t StartAddr = MCFN->getEntryBlock()->getStartAddr();

  TheFunction = getFunction(StartAddr);
  TheFunction->setDoesNotAlias(1);
  TheFunction->setDoesNotCapture(1);

  // Create the entry and exit basic blocks.
  TheBB =
      BasicBlock::Create(*Ctx, "entry_fn_" + utohexstr(StartAddr), TheFunction);
  ExitBB =
      BasicBlock::Create(*Ctx, "exit_fn_" + utohexstr(StartAddr), TheFunction);

  // From now on we insert in the entry basic block.
  Builder->SetInsertPoint(TheBB);

  if (EnableRegSetDiff) {
    Value *SavedRegSet = Builder->CreateAlloca(DRS.getRegSetType());
    //Value *RegSetArg = &TheFunction->getArgumentList().front();
    Value *RegSetArg = TheFunction->arg_begin();

    // First, save the previous regset in the entry block.
    Builder->CreateStore(Builder->CreateLoad(RegSetArg), SavedRegSet);

    // Second, insert a call to the diff function, in a separate exit block.
    // Move the return to that block, and branch to it from ExitBB.
    BasicBlock *DiffExitBB = BasicBlock::Create(
        *Ctx, "diff_exit_fn_" + utohexstr(StartAddr), TheFunction);

    DCIRBuilder ExitBBBuilder(DiffExitBB);

    Value *FnAddr = ExitBBBuilder.CreateIntToPtr(
        ExitBBBuilder.getInt64(reinterpret_cast<uint64_t>(StartAddr)),
        ExitBBBuilder.getInt8PtrTy());

    ExitBBBuilder.CreateCall(DRS.getOrCreateRegSetDiffFunction(),
                             {FnAddr, SavedRegSet, RegSetArg});
    ReturnInst::Create(*Ctx, DiffExitBB);
    BranchInst::Create(DiffExitBB, ExitBB);
  } else {
    // Create a ret void in the exit basic block.
    ReturnInst::Create(*Ctx, ExitBB);
  }

  // Create a br from the entry basic block to the first basic block, at
  // StartAddr.
  Builder->CreateBr(getOrCreateBasicBlock(StartAddr));

  DRS.SwitchToFunction(TheFunction);
}

void DCInstrSema::prepareBasicBlockForInsertion(BasicBlock *BB) {
  assert((BB->size() == 2 && isa<UnreachableInst>(++BB->begin())) &&
         "Several BBs at the same address?");
  BB->begin()->eraseFromParent();
  BB->begin()->eraseFromParent();
}

void DCInstrSema::SwitchToBasicBlock(const MCBasicBlock *MCBB) {
  TheMCBB = MCBB;
  SwitchToBasicBlock(getBasicBlockStartAddress());
}

void DCInstrSema::SwitchToBasicBlock(uint64_t BeginAddr) {
  TheBB = getOrCreateBasicBlock(BeginAddr);
  prepareBasicBlockForInsertion(TheBB);

  Builder->SetInsertPoint(TheBB);

  DRS.SwitchToBasicBlock(TheBB);
  // FIXME: we need to keep the unreachable+trap when the basic block is 0-inst.

  // The PC at the start of the basic block is known, just set it.
  // FIXME: AArch64 can't access the PC directly
  //  unsigned PC = DRS.MRI.getProgramCounter();
  //  setReg(PC, ConstantInt::get(DRS.getRegType(PC), BeginAddr));
}

uint64_t DCInstrSema::getBasicBlockStartAddress() const {
  assert(TheMCBB && "Getting start address without an MC BasicBlock");
  return TheMCBB->getStartAddr();
}

uint64_t DCInstrSema::getBasicBlockEndAddress() const {
  assert(TheMCBB && "Getting end address without an MC BasicBlock");
  return TheMCBB->getEndAddr();
}

Function *DCInstrSema::getFunction(uint64_t Addr) {
  std::string Name = "fn_" + utohexstr(Addr);
  TheModule->getOrInsertFunction(Name, FuncType);
  return TheModule->getFunction(Name);
}

BasicBlock *DCInstrSema::getOrCreateBasicBlock(uint64_t Addr) {
  BasicBlock *&BB = BBByAddr[Addr];
  if (!BB) {
    BB = BasicBlock::Create(*Ctx, "bb_" + utohexstr(Addr), TheFunction);
    DCIRBuilder BBBuilder(BB);
    BBBuilder.CreateCall(Intrinsic::getDeclaration(TheModule, Intrinsic::trap));
    BBBuilder.CreateUnreachable();
  }
  return BB;
}

BasicBlock *DCInstrSema::insertCallBB(Value *Target) {
  BasicBlock *CallBB =
      BasicBlock::Create(*Ctx, TheBB->getName() + "_call", TheFunction);
  //Value *RegSetArg = &TheFunction->getArgumentList().front();
  Value *RegSetArg = TheFunction->arg_begin();
  DCIRBuilder CallBuilder(CallBB);
  CallBuilder.CreateCall(Target, {RegSetArg});
  Builder->CreateBr(CallBB);
  assert(Builder->GetInsertPoint() == TheBB->end() &&
         "Call basic blocks can't be inserted at the middle of a basic block!");
  StringRef BBName = TheBB->getName();
  BBName = BBName.substr(0, BBName.find_first_of("_c"));
  TheBB = BasicBlock::Create(
      *Ctx, BBName + "_c" + utohexstr(CurrentInst->Address), TheFunction);
  DRS.FinalizeBasicBlock();
  DRS.SwitchToBasicBlock(TheBB);
  Builder->SetInsertPoint(TheBB);
  CallBuilder.CreateBr(TheBB);
  CallBBs.push_back(CallBB);
  // FIXME: Insert return address checking, to unwind back to the translator if
  // the call returned to an unexpected address.
  return CallBB;
}

Value *DCInstrSema::insertTranslateAt(Value *OrigTarget) {
  void *CBPtr = DynTranslateAtCBPtr;
  if (!CBPtr) {
    // If we don't have access to the dynamic translate_at function, jump to
    // some bad address.
    // FIXME: A better target would be a dedicated IR function with just trap+
    // unreachable, which would make it possible to debug more easily.
    // FIXME: We should be able generate a table with all possible call targets
    // from the symbol table.
    CBPtr = reinterpret_cast<void *>(0xDEAD);
  }

  FunctionType *CallbackType = FunctionType::get(
      FuncType->getPointerTo(), Builder->getInt8PtrTy(), false);
  return Builder->CreateCall(
      DRS.getCallTargetForExtFn(CallbackType, CBPtr),
      {Builder->CreateIntToPtr(OrigTarget, Builder->getInt8PtrTy())});
}

void DCInstrSema::insertCall(Value *CallTarget) {
  if (ConstantInt *CI = dyn_cast<ConstantInt>(CallTarget)) {
    uint64_t Target = CI->getValue().getZExtValue();
    CallTarget = getFunction(Target);
  } else {
    CallTarget = insertTranslateAt(CallTarget);
  }
  insertCallBB(CallTarget);
}

void DCInstrSema::translateBinOp(Instruction::BinaryOps Opc) {
  Value *V1 = getNextOperand();
  Value *V2 = getNextOperand();
  if (Instruction::isShift(Opc) && V2->getType() != V1->getType())
    V2 = Builder->CreateZExt(V2, V1->getType());
  registerResult(Builder->CreateBinOp(Opc, V1, V2));
}

void DCInstrSema::translateCastOp(Instruction::CastOps Opc) {
  Type *ResType = nullptr;
  Value *Val = getNextOperand();
  if (ResEVT.getSimpleVT() == MVT::Untyped) {
    ResType = Val->getType();
  } else {
    ResType = ResEVT.getTypeForEVT(*Ctx);
  }
  registerResult(Builder->CreateCast(Opc, Val, ResType));
}

bool DCInstrSema::translateInst(const MCDecodedInst &DecodedInst,
                                DCTranslatedInst &TranslatedInst) {
  CurrentInst = &DecodedInst;
  CurrentTInst = &TranslatedInst;
  DRS.SwitchToInst(DecodedInst);

  if (CurrentInst->Address == 0x101AC6BC8) {
    assert(true);
  }

  if (EnableInstAddrSave) {
    ConstantInt *CurIVal =
        Builder->getInt64(reinterpret_cast<uint64_t>(CurrentInst->Address));
    Value *CurIPtr = ConstantExpr::getIntToPtr(
        Builder->getInt64(reinterpret_cast<uint64_t>(&__llvm_dc_current_instr)),
        Builder->getInt64Ty()->getPointerTo());
    Builder->CreateStore(CurIVal, CurIPtr, true);
  }

  Idx = OpcodeToSemaIdx[CurrentInst->Inst.getOpcode()];
  DEBUG(errs() << "[+]Idx: " << Idx << "\n");
  if (!translateTargetInst()) {
    if (Idx == 0)
      return false;

    {
      // Increment the PC before anything.
      // FIXME: AArch64 can't access the PC
      //      Value *OldPC = getReg(DRS.MRI.getProgramCounter());
      //      setReg(DRS.MRI.getProgramCounter(),
      //             Builder->CreateAdd(
      //                 OldPC, ConstantInt::get(OldPC->getType(),
      //                 CurrentInst->Size)));
    }

    while ((Opcode = Next()) != DCINS::END_OF_INSTRUCTION) {
      DEBUG(errs() << "[+]Opcode: " << Opcode << "\n");
      if (translateOpcode(Opcode)) continue;
      errs() << "[!]CurrentInst->Address: " << utohexstr(CurrentInst->Address) << "\n";
      break;
    }
  }

  Vals.clear();
  CurrentInst = nullptr;
  CurrentTInst = nullptr;
  return true;
}

// Change translateOpcode type to boolean to judge whether translate successfully or not. 
bool DCInstrSema::translateOpcode(unsigned Opcode) {
  ResEVT = NextVT();
  if (Opcode >= ISD::BUILTIN_OP_END && Opcode < DCINS::DC_OPCODE_START) {
    translateTargetOpcode();
    return true;
  }
  switch (Opcode) {
  case ISD::ADD:
    translateBinOp(Instruction::Add);
    break;
  case ISD::FADD:
    translateBinOp(Instruction::FAdd);
    break;
  case ISD::SUB:
    translateBinOp(Instruction::Sub);
    break;
  case ISD::FSUB:
    translateBinOp(Instruction::FSub);
    break;
  case ISD::MUL:
    translateBinOp(Instruction::Mul);
    break;
  case ISD::FMUL:
    translateBinOp(Instruction::FMul);
    break;
  case ISD::UDIV:
    translateBinOp(Instruction::UDiv);
    break;
  case ISD::SDIV:
    translateBinOp(Instruction::SDiv);
    break;
  case ISD::FDIV:
    translateBinOp(Instruction::FDiv);
    break;
  case ISD::UREM:
    translateBinOp(Instruction::URem);
    break;
  case ISD::SREM:
    translateBinOp(Instruction::SRem);
    break;
  case ISD::FREM:
    translateBinOp(Instruction::FRem);
    break;
  case ISD::SHL:
    translateBinOp(Instruction::Shl);
    break;
  case ISD::SRL:
    translateBinOp(Instruction::LShr);
    break;
  case ISD::SRA:
    translateBinOp(Instruction::AShr);
    break;
  case ISD::AND:
    translateBinOp(Instruction::And);
    break;
  case ISD::OR:
    translateBinOp(Instruction::Or);
    break;
  case ISD::XOR:
    translateBinOp(Instruction::Xor);
    break;

  case ISD::TRUNCATE:
    translateCastOp(Instruction::Trunc);
    break;
  case ISD::BITCAST:
    translateCastOp(Instruction::BitCast);
    break;
  case ISD::ZERO_EXTEND:
    translateCastOp(Instruction::ZExt);
    break;
  case ISD::SIGN_EXTEND:
    translateCastOp(Instruction::SExt);
    break;
  case ISD::FP_TO_UINT:
    translateCastOp(Instruction::FPToUI);
    break;
  case ISD::FP_TO_SINT:
    translateCastOp(Instruction::FPToSI);
    break;
  case ISD::UINT_TO_FP:
    translateCastOp(Instruction::UIToFP);
    break;
  case ISD::SINT_TO_FP:
    translateCastOp(Instruction::SIToFP);
    break;
  case ISD::FP_ROUND:
    translateCastOp(Instruction::FPTrunc);
    break;
  case ISD::FP_EXTEND:
    translateCastOp(Instruction::FPExt);
    break;

  case ISD::FSQRT: {
    Value *V = getNextOperand();
    registerResult(Builder->CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::sqrt, V->getType()),
        {V}));
    break;
  }

  case ISD::INSERT_VECTOR_ELT: {
    Value *Vec = getNextOperand();
    Value *Val = getNextOperand();
    Value *Idx = getNextOperand();
    registerResult(Builder->CreateInsertElement(Vec, Val, Idx));
    break;
  }

  case ISD::EXTRACT_VECTOR_ELT: {
    Value *Val = getNextOperand();
    Value *Idx = getNextOperand();
    registerResult(Builder->CreateExtractElement(Val, Idx));
    break;
  }

  case ISD::SMUL_LOHI: {
    EVT Re2EVT = NextVT();
    IntegerType *LoResType = cast<IntegerType>(ResEVT.getTypeForEVT(*Ctx));
    IntegerType *HiResType = cast<IntegerType>(Re2EVT.getTypeForEVT(*Ctx));
    IntegerType *ResType = IntegerType::get(*Ctx, LoResType->getBitWidth() +
                                                      HiResType->getBitWidth());
    Value *Op1 = getNextOperand(), *Op2 = getNextOperand();
    Value *Full = Builder->CreateMul(Builder->CreateSExt(Op1, ResType),
                                     Builder->CreateSExt(Op2, ResType));
    registerResult(Builder->CreateTrunc(Full, LoResType));
    registerResult(Builder->CreateTrunc(
        Builder->CreateLShr(Full, LoResType->getBitWidth()), HiResType));
    break;
  }
  case ISD::UMUL_LOHI: {
    EVT Re2EVT = NextVT();
    IntegerType *LoResType = cast<IntegerType>(ResEVT.getTypeForEVT(*Ctx));
    IntegerType *HiResType = cast<IntegerType>(Re2EVT.getTypeForEVT(*Ctx));
    IntegerType *ResType = IntegerType::get(*Ctx, LoResType->getBitWidth() +
                                                      HiResType->getBitWidth());
    Value *Op1 = getNextOperand(), *Op2 = getNextOperand();
    Value *Full = Builder->CreateMul(Builder->CreateZExt(Op1, ResType),
                                     Builder->CreateZExt(Op2, ResType));
    registerResult(Builder->CreateTrunc(Full, LoResType));
    registerResult(Builder->CreateTrunc(
        Builder->CreateLShr(Full, LoResType->getBitWidth()), HiResType));
    break;
  }
  case ISD::LOAD: {
    Value *Ptr = getNextOperand();
    Type *ResType = nullptr;
    if (ResEVT.getSimpleVT() == MVT::Untyped) {
      ResType = Ptr->getType();
    } else {
      ResType = ResEVT.getTypeForEVT(*Ctx);
    }
    if (!Ptr->getType()->isPointerTy())
      Ptr = Builder->CreateIntToPtr(Ptr, ResType->getPointerTo());
    assert(Ptr->getType()->getPointerElementType() == ResType &&
           "Mismatch between a LOAD's address operand and return type!");
    registerResult(Builder->CreateAlignedLoad(Ptr, 1));
    break;
  }
  case ISD::STORE: {
    Value *Val = getNextOperand();
    Value *Ptr = getNextOperand();
    Type *ValPtrTy = Val->getType()->getPointerTo();
    Type *PtrTy = Ptr->getType();
    if (!PtrTy->isPointerTy())
      Ptr = Builder->CreateIntToPtr(Ptr, ValPtrTy);
    else if (PtrTy != ValPtrTy)
      Ptr = Builder->CreateBitCast(Ptr, ValPtrTy);
    Builder->CreateAlignedStore(Val, Ptr, 1);
    break;
  }
  case ISD::BRIND: {
    Value *Op1 = getNextOperand();
    setReg(DRS.MRI.getProgramCounter(), Op1);
    // FIXME: this should be only a branch!?
    insertCall(Op1);
    Builder->CreateBr(ExitBB);
    break;
  }
  case ISD::BR: {
    Value *Op1 = getNextOperand();
    uint64_t Target = cast<ConstantInt>(Op1)->getValue().getZExtValue();
    // FIXME: can't access program counter
    // setReg(DRS.MRI.getProgramCounter(), Op1);
    Builder->CreateBr(getOrCreateBasicBlock(Target));
    break;
  }
  case ISD::TRAP: {
    Builder->CreateCall(Intrinsic::getDeclaration(TheModule, Intrinsic::trap));
    break;
  }
  case DCINS::PUT_RC: {
    unsigned MIOperandNo = Next();
    unsigned RegNo = getRegOp(MIOperandNo);
    Value *Res = getNextOperand();
    Type *RegType = DRS.getRegType(RegNo);
    if (Res->getType()->isPointerTy())
      Res = Builder->CreatePtrToInt(Res, RegType);
    if (!Res->getType()->isIntegerTy())
      Res = Builder->CreateBitCast(
          Res,
          IntegerType::get(*Ctx, Res->getType()->getPrimitiveSizeInBits()));
    if (Res->getType()->getPrimitiveSizeInBits() <
        RegType->getPrimitiveSizeInBits()) {
      // FIXME: in AArch64 we do not insert bits???
      Res = Builder->CreateZExt(Res, RegType);
      //        Res = DRS.insertBitsInValue(getReg(RegNo), Res);
    }

    assert(Res->getType() == RegType);
    setReg(RegNo, Res);
    CurrentTInst->addRegOpDef(MIOperandNo, Res);
    break;
  }
  case DCINS::PUT_REG: {
    unsigned RegNo = Next();
    Value *Res = getNextOperand();
    setReg(RegNo, Res);
    CurrentTInst->addImpDef(RegNo, Res);
    break;
  }
  case DCINS::GET_RC: {
    unsigned MIOperandNo = Next();
    Type *ResType = NULL;
    if (ResEVT.getEVTString() != "Untyped") {
      ResType = ResEVT.getTypeForEVT(*Ctx);
    }

    Value *Reg = getReg(getRegOp(MIOperandNo));
    if (ResType && ResType->getPrimitiveSizeInBits() <
                       Reg->getType()->getPrimitiveSizeInBits())
      Reg = Builder->CreateTrunc(
          Reg, IntegerType::get(*Ctx, ResType->getPrimitiveSizeInBits()));
    if (ResType && !ResType->isIntegerTy())
      Reg = Builder->CreateBitCast(Reg, ResType);
    registerResult(Reg);
    CurrentTInst->addRegOpUse(MIOperandNo, Reg);
    break;
  }
  case DCINS::GET_REG: {
    unsigned RegNo = Next();
    Value *RegVal = getReg(RegNo);
    registerResult(RegVal);
    CurrentTInst->addImpUse(RegNo, RegVal);
    break;
  }
  case DCINS::CUSTOM_OP: {
    unsigned OperandType = Next(), MIOperandNo = Next();
    translateOperand(OperandType, MIOperandNo);
    CurrentTInst->addOpUse(MIOperandNo, OperandType, Vals.back());
    break;
  }
  case DCINS::CONSTANT_OP: {
    unsigned MIOperandNo = Next();
    Type *ResType = ResEVT.getTypeForEVT(*Ctx);
    Value *Cst =
        ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
    registerResult(Cst);
    CurrentTInst->addImmOpUse(MIOperandNo, Cst);
    break;
  }
  case DCINS::MOV_CONSTANT: {
    uint64_t ValIdx = Next();
    Type *ResType = nullptr;
    if (ResEVT.getSimpleVT() == MVT::iPTR)
      // FIXME: what should we do here? Maybe use DL's intptr type?
      ResType = Builder->getInt64Ty();
    else
      ResType = ResEVT.getTypeForEVT(*Ctx);
    registerResult(ConstantInt::get(ResType, ConstantArray[ValIdx]));
    break;
  }
  case DCINS::IMPLICIT: {
    translateImplicit(Next());
    break;
  }
  case ISD::INTRINSIC_VOID: {
    Value *IndexV = getNextOperand();
    // FIXME: the intrinsics sdnodes have variable numbers of arguments.
    // FIXME: handle overloaded intrinsics, but how?
    if (ConstantInt *IndexCI = dyn_cast<ConstantInt>(IndexV)) {
      uint64_t IntID = IndexCI->getZExtValue();
      Value *IntDecl =
          Intrinsic::getDeclaration(TheModule, Intrinsic::ID(IntID));
      registerResult(Builder->CreateCall(IntDecl));
    } else {
      llvm_unreachable("Unable to translate non-constant intrinsic ID");
    }
    break;
  }
  case ISD::BSWAP: {
    Type *ResType = ResEVT.getTypeForEVT(*Ctx);
    Value *Op = getNextOperand();
    Value *IntDecl =
        Intrinsic::getDeclaration(TheModule, Intrinsic::bswap, ResType);
    registerResult(Builder->CreateCall(IntDecl, Op));
    break;
  }
  case ISD::CTLZ: {
    auto count = [this](Value *v) {
      uint64_t Width = v->getType()->getIntegerBitWidth();
      Value *Zero = Builder->getInt(APInt(Width, 0));
      Value *Max = Builder->getInt(APInt(Width, 0));
      for (uint64_t i = 0; i < Width; ++i) {
        Value *IsZero = Builder->CreateICmpEQ(v, Zero);
        Value *NoMax = Builder->CreateICmpEQ(Max, Zero);
        IsZero = Builder->CreateAnd(IsZero, NoMax);
        Max = Builder->CreateSelect(
            IsZero, Builder->getInt(APInt(Width, Width - i)), Max);
        v = Builder->CreateLShr(v, 1);
      }
      return Max;
    };

    if (ResEVT.isVector()) {
      Value *vec = getNextOperand();
      Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
      result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
      for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
        Value *elem = Builder->CreateExtractElement(vec, i);
        result = Builder->CreateInsertElement(result, count(elem), i);
      }
      registerResult(result);
    } else {
      Type *ResType = ResEVT.getTypeForEVT(*Ctx);
      Value *Op = getNextOperand();
      registerResult(count(Op));
    }

    break;
  }
  case ISD::FABS: {
    Value *Reg = getNextOperand();
    Value *Zero = ConstantFP::get(Reg->getType(), 0.0);
    Value *One_neg = ConstantFP::get(Reg->getType(), -1.0);
    Value *IsPos = Builder->CreateFCmpOGE(Reg, Zero);
    Value *Neg = Builder->CreateFMul(Reg, One_neg);
    Value *Result = Builder->CreateSelect(IsPos, Reg, Neg);
    registerResult(Result);
    break;
  }
  case ISD::FNEG: {
    Value *Neg = Builder->CreateFNeg(getNextOperand());
    registerResult(Neg);
    break;
  }
  case ISD::FNEARBYINT:
  case ISD::FTRUNC:
  case ISD::FROUND:
  case ISD::FRINT: {
    Value *Src = getNextOperand();
    std::vector<Value *> args;
    args.push_back(Src);
    std::vector<Type *> types;
    types.push_back(Src->getType());
    Value *Result = Builder->CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::round, types), args);
    registerResult(Result);
    break;
  }
  case ISD::FFLOOR: {
    Value *Src = getNextOperand();
    std::vector<Value *> args;
    args.push_back(Src);
    std::vector<Type *> types;
    types.push_back(Src->getType());
    Value *Result = Builder->CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::floor, types), args);
    registerResult(Result);
    break;
  }
  case ISD::FCEIL: {
    Value *Src = getNextOperand();
    std::vector<Value *> args;
    args.push_back(Src);
    std::vector<Type *> types;
    types.push_back(Src->getType());
    Value *Result = Builder->CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::ceil, types), args);
    registerResult(Result);
    break;
  }
  case ISD::ConstantFP: {
    registerResult(ConstantFP::get(ResEVT.getTypeForEVT(*Ctx), 0.0));
    break;
  }
  case ISD::MULHU: {
    Type *ExtType = IntegerType::get(
        *Ctx, 2 * ResEVT.getTypeForEVT(*Ctx)->getIntegerBitWidth());

    Value *LHS = getNextOperand();
    Value *RHS = getNextOperand();

    LHS = Builder->CreateZExt(LHS, ExtType);
    RHS = Builder->CreateZExt(RHS, ExtType);
    Value *Result = Builder->CreateMul(LHS, RHS);
    Result = Builder->CreateLShr(Result,
                                 Result->getType()->getIntegerBitWidth() / 2);
    Result = Builder->CreateTrunc(Result, ResEVT.getTypeForEVT(*Ctx));
    registerResult(Result);
    break;
  }
  case ISD::MULHS: {
    Type *ExtType = IntegerType::get(
        *Ctx, 2 * ResEVT.getTypeForEVT(*Ctx)->getIntegerBitWidth());

    Value *LHS = getNextOperand();
    Value *RHS = getNextOperand();

    LHS = Builder->CreateSExt(LHS, ExtType);
    RHS = Builder->CreateSExt(RHS, ExtType);
    Value *Result = Builder->CreateMul(LHS, RHS);
    Result = Builder->CreateLShr(Result,
                                 Result->getType()->getIntegerBitWidth() / 2);
    Result = Builder->CreateTrunc(Result, ResEVT.getTypeForEVT(*Ctx));
    registerResult(Result);
    break;
  }
  case ISD::BUILD_VECTOR: {
    assert(ResEVT.getSimpleVT().isVector());
    Value *Vector =
        Builder->getInt(APInt(ResEVT.getSimpleVT().getScalarSizeInBits() *
                                  ResEVT.getSimpleVT().getVectorNumElements(),
                              0));
    Vector = Builder->CreateBitCast(Vector, ResEVT.getTypeForEVT(*Ctx));
    registerResult(Vector);
    break;
  }
  case ISD::INTRINSIC_WO_CHAIN: {
    if (const ConstantInt *id = dyn_cast<const ConstantInt>(getNextOperand())) {
      translateTargetIntrinsic(id->getZExtValue());
    } else {
      llvm_unreachable("");
    }
    break;
  }
  // BugID: koubei_100745578
  case ISD::SABSDIFF:
  case ISD::UABSDIFF: {
    if (ResEVT.getSimpleVT().isVector()) {
      Value *zero = Builder->getInt(
          APInt(ResEVT.getVectorElementType().getSizeInBits(), 0));
      Value *minusOne = Builder->getInt(
          APInt(ResEVT.getVectorElementType().getSizeInBits(), -1));
      Value *op1 = getNextOperand();
      Value *op2 = getNextOperand();
      Value *result =
          Builder->getInt(APInt(ResEVT.getSimpleVT().getSizeInBits(), 0));
      result = Builder->CreateBitCast(result, op1->getType());
      for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements();
           ++i) {
        Value *elem1 = Builder->CreateExtractElement(op1, i);
        Value *elem2 = Builder->CreateExtractElement(op2, i);
        Value *elemResult = Builder->CreateSub(elem1, elem2);
        Value *elemPos = Builder->CreateICmpSGT(elemResult, zero);
        elemResult = Builder->CreateSelect(
            elemPos, elemResult, Builder->CreateMul(elemResult, minusOne));
        result = Builder->CreateInsertElement(result, elemResult, i);
      }
      registerResult(result);
    } else {
      Value *zero = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
      Value *minusOne = Builder->getInt(APInt(ResEVT.getSizeInBits(), -1));
      Value *op1 = getNextOperand();
      Value *op2 = getNextOperand();
      Value *result = Builder->CreateSub(op1, op2);
      Value *pos = Builder->CreateICmpSGT(result, zero);
      result = Builder->CreateSelect(pos, result,
                                     Builder->CreateMul(result, minusOne));
      registerResult(result);
    }
    break;
  }
  case ISD::UMAX: {
    assert(ResEVT.getSimpleVT().isVector());
    Value *op1 = getNextOperand();
    Value *op2 = getNextOperand();
    Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
    result = Builder->CreateBitCast(result, op1->getType());

    for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
      Value *elem1 = Builder->CreateExtractElement(op1, i);
      Value *elem2 = Builder->CreateExtractElement(op2, i);
      Value *cmp = Builder->CreateICmpUGT(elem1, elem2);
      Value *resElem = Builder->CreateSelect(cmp, elem1, elem2);
      result = Builder->CreateInsertElement(result, resElem, i);
    }
    registerResult(result);
    break;
  }
  case ISD::SMAX: {
    assert(ResEVT.getSimpleVT().isVector());
    Value *op1 = getNextOperand();
    Value *op2 = getNextOperand();
    Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
    result = Builder->CreateBitCast(result, op1->getType());

    for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
      Value *elem1 = Builder->CreateExtractElement(op1, i);
      Value *elem2 = Builder->CreateExtractElement(op2, i);
      Value *cmp = Builder->CreateICmpSGT(elem1, elem2);
      Value *resElem = Builder->CreateSelect(cmp, elem1, elem2);
      result = Builder->CreateInsertElement(result, resElem, i);
    }
    registerResult(result);
    break;
  }
  // TODO: check if this is always an extraction of the upper half of a vector
  case ISD::EXTRACT_SUBVECTOR: {
    Value *vec = getNextOperand();
    ConstantInt *constantInt = dyn_cast<ConstantInt>(getNextOperand());
    assert(constantInt);
    //          constantInt->dump();
    Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
    result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
    //          if (constantInt->getZExtValue() == 2) {
    for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
      Value *elem =
          Builder->CreateExtractElement(vec, ResEVT.getVectorNumElements() + i);
      result = Builder->CreateInsertElement(result, elem, i);
    }
    //          }
    registerResult(result);
    break;
  }
  case ISD::FMA: {
    Value *op1 = getNextOperand();
    Value *op2 = getNextOperand();
    Value *op3 = getNextOperand();

    if (ResEVT.isVector()) {
      for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
        Value *elem1 = Builder->CreateExtractElement(op1, i);
        Value *elem2 = Builder->CreateExtractElement(op2, i);
        Value *elem3 = Builder->CreateExtractElement(op3, i);

        Value *resElem = Builder->CreateFMul(elem1, elem2);
        resElem = Builder->CreateFAdd(resElem, elem3);

        op3 = Builder->CreateInsertElement(op3, resElem, i);
      }
    } else {
      op3 = Builder->CreateFAdd(op3, Builder->CreateFMul(op1, op2));
    }

    registerResult(op3);
    break;
  }
  case ISD::FMINNUM:
  case ISD::FMAXNUM:
  case ISD::FMINNAN:
  case ISD::FMAXNAN: {
    if (ResEVT.isVector()) {
      Value *vec1 = getNextOperand();
      Value *vec2 = getNextOperand();
      Value *res = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
      res = Builder->CreateBitCast(res, ResEVT.getTypeForEVT(*Ctx));

      for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
        Value *elem1 = Builder->CreateExtractElement(vec1, i);
        Value *elem2 = Builder->CreateExtractElement(vec2, i);
        Value *cmp = nullptr;
        switch (Opcode) {
        default:
          llvm_unreachable("");
        case ISD::FMAXNUM:
        case ISD::FMAXNAN: {
          cmp = Builder->CreateFCmpUGT(elem1, elem2);
          break;
        }
        case ISD::FMINNUM:
        case ISD::FMINNAN: {
          cmp = Builder->CreateFCmpULT(elem1, elem2);
          break;
        }
        }
        Value *resElem = Builder->CreateSelect(cmp, elem1, elem2);
        res = Builder->CreateInsertElement(res, resElem, i);
      }
      registerResult(res);
    } else {
      Value *op1 = getNextOperand();
      Value *op2 = getNextOperand();
      Value *cmp = nullptr;
      switch (Opcode) {
      default:
        llvm_unreachable("");
      case ISD::FMAXNUM:
      case ISD::FMAXNAN: {
        cmp = Builder->CreateFCmpUGT(op1, op2);
        break;
      }
      case ISD::FMINNUM:
      case ISD::FMINNAN: {
        cmp = Builder->CreateFCmpULT(op1, op2);
        break;
      }
      }
      Value *res = Builder->CreateSelect(cmp, op1, op2);
      registerResult(res);
    }
    break;
  }
  case ISD::CTPOP: {
    if (ResEVT.isVector()) {
      Value *vec = getNextOperand();
      Value *res = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
      res = Builder->CreateBitCast(res, ResEVT.getTypeForEVT(*Ctx));

      for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
        Value *elem = Builder->CreateExtractElement(vec, i);
        Value *resElem = Builder->getInt(
            APInt(ResEVT.getVectorElementType().getScalarSizeInBits(), 0));
        for (unsigned j = 0;
             j < ResEVT.getVectorElementType().getScalarSizeInBits(); ++j) {
          Value *cmp = Builder->CreateAnd(elem, 1);
          cmp = Builder->CreateICmpEQ(
              cmp,
              Builder->getInt(APInt(cmp->getType()->getScalarSizeInBits(), 1)));
          resElem = Builder->CreateAdd(
              resElem,
              Builder->CreateSelect(
                  cmp,
                  Builder->getInt(APInt(
                      ResEVT.getVectorElementType().getScalarSizeInBits(), 1)),
                  Builder->getInt(
                      APInt(ResEVT.getVectorElementType().getScalarSizeInBits(),
                            0))));
          elem = Builder->CreateLShr(elem, 1);
        }
        res = Builder->CreateInsertElement(res, resElem, i);
      }
      registerResult(res);
    } else {
      llvm_unreachable("not handled");
    }
    break;
  }
  case ISD::ROTR: {
    Value *op = getNextOperand();
    Value *r = getNextOperand();
    // TODO: perform rotate...
    registerResult(op);
    break;
  }
  case ISD::UMIN: {
    Value *op1 = getNextOperand();
    Value *op2 = getNextOperand();
    std::vector<Value *> args;
    args.push_back(op1);
    args.push_back(op2);

    std::vector<Type *> types;
    types.push_back(op1->getType());

    Value *result =
        Builder->CreateCall(Intrinsic::getDeclaration(
                                TheModule, Intrinsic::aarch64_neon_umin, types),
                            args);
    registerResult(result);
    break;
  }
  case ISD::SMIN: {
    Value *op1 = getNextOperand();
    Value *op2 = getNextOperand();
    std::vector<Value *> args;
    args.push_back(op1);
    args.push_back(op2);

    std::vector<Type *> types;
    types.push_back(op1->getType());

    Value *result =
        Builder->CreateCall(Intrinsic::getDeclaration(
                                TheModule, Intrinsic::aarch64_neon_smin, types),
                            args);
    registerResult(result);
    break;
  }
    //      case ISD::SMAX: {
    //          Value *op1 = getNextOperand();
    //          Value *op2 = getNextOperand();
    //          std::vector<Value*> args;
    //          args.push_back(op1);
    //          args.push_back(op2);
    //
    //          std::vector<Type*> types;
    //          types.push_back(op1->getType());
    //
    //          Value *result =
    //          Builder->CreateCall(Intrinsic::getDeclaration(TheModule,
    //          Intrinsic::aarch64_neon_smax, types), args);
    //          registerResult(result);
    //          break;
    //      }
    //      case ISD::UMAX: {
    //          Value *op1 = getNextOperand();
    //          Value *op2 = getNextOperand();
    //          std::vector<Value*> args;
    //          args.push_back(op1);
    //          args.push_back(op2);
    //
    //          std::vector<Type*> types;
    //          types.push_back(op1->getType());
    //
    //          Value *result =
    //          Builder->CreateCall(Intrinsic::getDeclaration(TheModule,
    //          Intrinsic::aarch64_neon_umax, types), args);
    //          registerResult(result);
    //          break;
    //      }
  default:
    // llvm_unreachable(
        errs() << "Unknown opcode found in semantics: " << Opcode << "\n";
        return false;
  }
  return true;
}

void DCInstrSema::translateOperand(unsigned OperandType, unsigned MIOperandNo) {
  // FIXME: We don't have target-independent operand types yet.
  translateCustomOperand(OperandType, MIOperandNo);
}
