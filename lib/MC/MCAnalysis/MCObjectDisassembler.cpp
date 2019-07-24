//===- lib/MC/MCObjectDisassembler.cpp ------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/Format.h"

#include "llvm/MC/MCObjectDisassembler.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"
#include "llvm/MC/MCAnalysis/MCFunction.h"
#include "llvm/MC/MCAnalysis/MCModule.h"
#include "llvm/MC/MCAnalysis/MCObjectSymbolizer.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/Object/MachO.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/Debug.h"
#include "llvm/BinaryFormat/MachO.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/LEB128.h"
#include <map>

using namespace llvm;
using namespace object;

#define DEBUG_TYPE "mccfg"

MCObjectDisassembler::MCObjectDisassembler(const ObjectFile &Obj,
                                           const MCDisassembler &Dis,
                                           const MCInstrAnalysis &MIA)
    : Obj(Obj), Dis(Dis), MIA(MIA), MOS(nullptr), Stripped(true) {
    if (const object::MachOObjectFile *MachO = dyn_cast<object::MachOObjectFile>(&Obj)) {
        ObjCFile = std::unique_ptr<ObjectiveCFile>(new ObjectiveCFile((object::MachOObjectFile*)MachO));
    }
}

const MCObjectDisassembler::MemoryRegion &
MCObjectDisassembler::getRegionFor(uint64_t Addr) {
    if (Stripped) {
        for (std::vector<MemoryRegion>::iterator it = SectionRegions.begin(); it != SectionRegions.end(); ++it) {
            if (Addr >= it->Addr && Addr < (it->Addr + it->Bytes.size())) {
                uint64_t Next = 0;
                for (unsigned i = 0; i < FunctionStarts.size(); ++i) {
                    if (FunctionStarts[i] <= Addr && FunctionStarts[i + 1] > Addr) {
                        Next = FunctionStarts[i + 1];
                        break;
                    }
                }
                if (Next) {
                    ArrayRef<uint8_t> data = it->Bytes.slice((Addr - it->Addr), (Next - Addr));
//                auto Region = std::make_shared<MemoryRegion>(Addr, data);
                    //FIXME: What about releasing??????!!!!!!
                    MemoryRegion *Region = new MemoryRegion(Addr, data);
                    return *Region;
                } else {
//                    ArrayRef<uint8_t> data = it->Bytes.slice(Addr - it->Addr);
//                    MemoryRegion *Region = new MemoryRegion(Addr, data);
//                    return *Region;
                    auto Region =
                            std::lower_bound(SectionRegions.begin(), SectionRegions.end(), Addr,
                                             [](const MemoryRegion &L, uint64_t Addr) {
                                                 return L.Addr + L.Bytes.size() <= Addr;
                                             });
                    if (Region != SectionRegions.end())
                    if (Region->Addr <= Addr)
                        return *Region;
                }
            }
        }
        return FallbackRegion;
    }
  auto Region =
      std::lower_bound(SectionRegions.begin(), SectionRegions.end(), Addr,
                       [](const MemoryRegion &L, uint64_t Addr) {
                         return L.Addr + L.Bytes.size() <= Addr;
                       });
  if (Region != SectionRegions.end())
    if (Region->Addr <= Addr)
      return *Region;
  return FallbackRegion;
}

MCModule *MCObjectDisassembler::buildEmptyModule() {
  MCModule *Module = new MCModule;
  return Module;
}

MCModule *MCObjectDisassembler::buildModule() {
  MCModule *Module = buildEmptyModule();
  errs()<<"mcobjectDisassembler : \n";
  if (SectionRegions.empty()) {
    for (const SectionRef &Section : Obj.sections()) {
        StringRef SectionName;
        Section.getName(SectionName);

        errs() << "Section: " << SectionName << "\n";
      bool isText = Section.isText();
      uint64_t StartAddr = Section.getAddress();
      uint64_t SecSize = Section.getSize();

      // FIXME
      if (!SecSize)
        continue;
      if (MOS)
        StartAddr = MOS->getEffectiveLoadAddr(StartAddr);
      if (!isText)
        continue;

      StringRef Contents;
      if (Section.getContents(Contents))
        continue;
      SectionRegions.emplace_back(
          StartAddr,
          ArrayRef<uint8_t>(reinterpret_cast<const uint8_t *>(Contents.data()),
                            Contents.size()));
    }
    std::sort(SectionRegions.begin(), SectionRegions.end(),
              [](const MemoryRegion &L, const MemoryRegion &R) {
                return L.Addr < R.Addr;
              });
  }

  buildCFG(Module);
  return Module;
}

namespace {
  struct BBInfo;
  typedef SmallPtrSet<BBInfo*, 2> BBInfoSetTy;

  struct BBInfo {
    uint64_t BeginAddr;
    uint64_t SizeInBytes;
    MCBasicBlock *BB;
    std::vector<MCDecodedInst> Insts;
    MCObjectDisassembler::AddressSetTy SuccAddrs;

    BBInfo() : BeginAddr(0), SizeInBytes(0), BB(nullptr) {}
  };
}

static void RemoveDupsFromAddressVector(MCObjectDisassembler::AddressSetTy &V) {
  std::sort(V.begin(), V.end());
  V.erase(std::unique(V.begin(), V.end()), V.end());
}

void MCObjectDisassembler::buildCFG(MCModule *Module) {
  AddressSetTy CallTargets;
  AddressSetTy TailCallTargets;

    bool S = true;
    Stripped = false;

//  for (const SymbolRef &Symbol : Obj.symbols()) {
//    SymbolRef::Type SymType = Symbol.getType();
//    DEBUG(errs() << Symbol.getName().get() << "\n");
//    if (SymType == SymbolRef::ST_Function) {
//      ErrorOr<uint64_t> SymAddrOrErr = Symbol.getAddress();
//      if (SymAddrOrErr.getError())
//        continue;
//      uint64_t SymAddr = *SymAddrOrErr;
//      if (MOS)
//        SymAddr = MOS->getEffectiveLoadAddr(SymAddr);
//      if (getRegionFor(SymAddr).Bytes.empty())
//        continue;
//      createFunction(Module, SymAddr, CallTargets, TailCallTargets);
//      S = false;
//    }
//  }

    Stripped = S;

    if (Stripped) {
        FunctionStarts = findFunctionStarts();
        RemoveDupsFromAddressVector(FunctionStarts);

        for (AddressSetTy::iterator it = FunctionStarts.begin(); it != FunctionStarts.end(); ++it) {
        //FIXME: remove this
//            if (*it < 0x100D5B894   ) {
//                continue;
//            }
            createFunction(Module, *it, CallTargets, TailCallTargets);
        }
    }

  RemoveDupsFromAddressVector(CallTargets);
  RemoveDupsFromAddressVector(TailCallTargets);

  AddressSetTy NewCallTargets;

  while (!NewCallTargets.empty()) {
    // First, create functions for all the previously found targets
    for (uint64_t CallTarget : CallTargets) {
      if (MOS)
        CallTarget = MOS->getEffectiveLoadAddr(CallTarget);
      createFunction(Module, CallTarget, NewCallTargets, TailCallTargets);
    }
    // Next, forget about those targets, since we just handled them.
    CallTargets.clear();
    RemoveDupsFromAddressVector(NewCallTargets);
    CallTargets = NewCallTargets;
  }
}

namespace {
  class AddrPrettyStackTraceEntry : public PrettyStackTraceEntry {
  public:
    uint64_t StartAddr;
    const char *Kind;
    AddrPrettyStackTraceEntry(uint64_t StartAddr, const char *Kind)
      : PrettyStackTraceEntry(), StartAddr(StartAddr), Kind(Kind) {}

    void print(raw_ostream &OS) const override {
      OS << "MC CFG: Disassembling " << Kind << " at address "
         << utohexstr(StartAddr) << "\n";
    }
  };
} // end anonymous namespace

// Basic idea of the disassembly + discovery:
//
// start with the wanted address, insert it in the worklist
// while worklist not empty, take next address in the worklist:
// - check if atom exists there
//   - if middle of atom:
//     - split basic blocks referencing the atom
//     - look for an already encountered BBInfo (using a map<atom, bbinfo>)
//       - if there is, split it (new one, fallthrough, move succs, etc..)
//   - if start of atom: nothing else to do
//   - if no atom: create new atom and new bbinfo
// - look at the last instruction in the atom, add succs to worklist
// for all elements in the worklist:
// - create basic block, update preds/succs, etc..
//
void MCObjectDisassembler::disassembleFunctionAt(
    MCModule *Module, MCFunction *MCFN, uint64_t BBBeginAddr,
    AddressSetTy &CallTargets, AddressSetTy &TailCallTargets) {
  std::map<uint64_t, BBInfo> BBInfos;

  typedef SmallSetVector<uint64_t, 16> AddrWorklistTy;

  AddrWorklistTy Worklist;

  DEBUG(dbgs() << "Starting CFG at " << utohexstr(BBBeginAddr) << "\n");

    AddressSetTy::iterator startIt = std::find(FunctionStarts.begin(), FunctionStarts.end(), BBBeginAddr);
    if (startIt == FunctionStarts.end()) {
        llvm_unreachable("");
    }
    AddressSetTy::iterator endIt = startIt;
    std::advance(endIt, 1);

    uint64_t startAddr = *startIt;
    uint64_t endAddr = (endIt == FunctionStarts.end()) ? INT64_MAX : *endIt;

    if (BBBeginAddr == 0x10001BBF4) {
        assert(true);
    }

  Worklist.insert(BBBeginAddr);
  for (size_t wi = 0; wi < Worklist.size(); ++wi) {
    const uint64_t BeginAddr = Worklist[wi];

    AddrPrettyStackTraceEntry X(BeginAddr, "Basic Block");

    DEBUG(dbgs() << "Looking for block at " << utohexstr(BeginAddr) << "\n");

    // Look for a BB at BeginAddr.
    auto BeforeIt = std::upper_bound(
        BBInfos.begin(), BBInfos.end(), BeginAddr,
        [](uint64_t Addr, const std::pair<uint64_t, BBInfo> &BBI) {
          return Addr < BBI.second.BeginAddr+BBI.second.SizeInBytes;
        });

    assert((BeforeIt == BBInfos.end() || BeforeIt->first != BeginAddr) &&
           "Visited same basic block twice!");

    // Found a BB containing BeginAddr, we have to split it.
    if (BeforeIt != BBInfos.end() && BeforeIt->first < BeginAddr) {

      BBInfo &BeforeBB = BeforeIt->second;
      DEBUG(dbgs() << "Found block at " << utohexstr(BeforeBB.BeginAddr)
                   << ", needs splitting at " << utohexstr(BeginAddr) << "\n");

      assert(BeginAddr < BeforeBB.BeginAddr + BeforeBB.SizeInBytes &&
             "Address isn't inside block?");

      BBInfo &NewBB = BBInfos[BeginAddr];
      NewBB.BeginAddr = BeginAddr;

      auto SplitInst = BeforeBB.Insts.end();
      for (auto I = BeforeBB.Insts.begin(), E = BeforeBB.Insts.end(); I != E;
           ++I) {
        if (BeginAddr == I->Address) {
          SplitInst = I;
          break;
        }
      }

      assert(SplitInst != BeforeBB.Insts.end() &&
             "Split point does not fall on an instruction boundary!");

      // FIXME: use a list instead for free splicing?

      // Splice the remaining instructions to the new block.
      // While SplitInst is still valid, decrease the size to match.
      const uint64_t SplitOffset = SplitInst->Address - BeforeBB.BeginAddr;
      NewBB.SizeInBytes = BeforeBB.SizeInBytes - SplitOffset;
      BeforeBB.SizeInBytes = SplitOffset;

      // Now do the actual splicing out of BeforeBB.
      NewBB.Insts.insert(NewBB.Insts.begin(), SplitInst, BeforeBB.Insts.end());
      BeforeBB.Insts.erase(SplitInst, BeforeBB.Insts.end());

      // Move the successors to the new block.
      std::swap(NewBB.SuccAddrs, BeforeBB.SuccAddrs);

      BeforeBB.SuccAddrs.push_back(BeginAddr);
    } else {
      // If we didn't find a BB, then we have to disassemble to create one!
      const MemoryRegion &Region = getRegionFor(BeginAddr);
      if (Region.Bytes.empty()) {
        //report_fatal_error(("No suitable region for disassembly at 0x" +
        errs() << "No suitable region for disassembly at 0x" <<
                            utohexstr(BeginAddr).c_str() << "\n";
        continue;
      }
      const uint64_t EndRegion = Region.Addr + Region.Bytes.size();

      uint64_t EndAddr = EndRegion;

      // We want to stop before the next BB and have a fallthrough to it.
      if (BeforeIt != BBInfos.end())
        EndAddr = std::min(EndAddr, BeforeIt->first);

      BBInfo &BBI = BBInfos[BeginAddr];
      BBI.BeginAddr = BeginAddr;

      assert(BBI.Insts.empty() && "Basic Block already exists!");

      DEBUG(dbgs() << "No existing block found, starting disassembly from "
                   << utohexstr(Region.Addr) << " to "
                   << utohexstr(Region.Addr + Region.Bytes.size()) << "\n");

      auto AddInst = [&](MCInst &I, uint64_t Addr, uint64_t Size) {
        const uint64_t NextAddr = BBI.BeginAddr + BBI.SizeInBytes;
        assert(NextAddr == Addr);
        BBI.Insts.emplace_back(I, NextAddr, Size);
        BBI.SizeInBytes += Size;
      };

      uint64_t InstSize;

      for (uint64_t Addr = BeginAddr; Addr < EndAddr; Addr += InstSize) {

        MCInst Inst;
        if (Dis.getInstruction(Inst, InstSize,
                               Region.Bytes.slice(Addr - Region.Addr), Addr,
                               nulls(), nulls())) {

        } else {
          DEBUG(dbgs() << "Failed disassembly at " << utohexstr(Addr) << "!\n");
          break;
        }

        uint64_t BranchTarget;
          bool isTailcall = false;
          bool lastInst = false;

          if (Addr == 4297609936) {
              isTailcall = false;
          }

          if (MIA.evaluateBranch(Inst, Addr, InstSize, BranchTarget) && (startAddr <= Addr && Addr <= endAddr)) {
              if (!MIA.isCall(Inst)) {
                  if (BranchTarget && !(startAddr <= BranchTarget && BranchTarget <= endAddr)) {
                      bool isDefined = false;
                      for (AddressSetTy::iterator startIt = FunctionStarts.begin(); startIt != FunctionStarts.end(); ++startIt) {
                          AddressSetTy::iterator endIt = startIt;
                          std::advance(endIt, 1);
                          if (endIt == FunctionStarts.end())
                              continue;
                          if ((*startIt) <= BranchTarget && BranchTarget <= (*endIt)) {
                              isDefined = true;
                              break;
                          }
                      }
                      if (isDefined && Inst.getOpcode() == 104) {
                          isTailcall = true;
                      }

                  }
              }
          }

          if (isTailcall) {
              Inst.setOpcode(125);
          }

          AddInst(Inst, Addr, InstSize);

          if (isTailcall) {
              MCInst retInst;
              MCOperand retOp;
              retOp.createReg(2);
              retInst.setOpcode(1343);
              AddInst(retInst, Addr + InstSize, 4);

              if ((Addr + InstSize) == endAddr) {
                  lastInst = true;
              } else {
                  lastInst = true;
              }
          }
        if (Inst.getOpcode() == 1343) {
            assert(true);
        }

        if (MIA.evaluateBranch(Inst, Addr, InstSize, BranchTarget)) {
          DEBUG(dbgs() << "Found branch to " << utohexstr(BranchTarget)
                       << "!\n");
          if (MIA.isCall(Inst)) {
            DEBUG(dbgs() << "Found call!\n");
            CallTargets.push_back(BranchTarget);
          } else {
              if (checkBranch(Inst, BranchTarget)) {
//                  Inst.setOpcode(125);
//                  CallTargets.push_back(BranchTarget);
              }
          }
        }

        if (MIA.isTerminator(Inst) || lastInst) {
          DEBUG(dbgs() << "Found terminator!\n");
          // Now we have a complete basic block, add successors.

          // Add the fallthrough block, and mark it for visiting.
          if (MIA.isConditionalBranch(Inst)) {
            BBI.SuccAddrs.push_back(Addr + InstSize);
            Worklist.insert(Addr + InstSize);
          }
          // If the terminator is a branch, add the target block.
          if (MIA.isBranch(Inst)) {
            uint64_t BranchTarget;
            if (MIA.evaluateBranch(Inst, Addr, InstSize, BranchTarget)) {
              StringRef ExtFnName;
              if (MOS &&
                  !(ExtFnName = MOS->findExternalFunctionAt(BranchTarget))
                       .empty()) {
                TailCallTargets.push_back(BranchTarget);
                CallTargets.push_back(BranchTarget);
              } else {
                BBI.SuccAddrs.push_back(BranchTarget);
                Worklist.insert(BranchTarget);
              }
            }
          }
          break;
        }
      }
    }
  }

  // First, create all blocks.
  for (size_t wi = 0, we = Worklist.size(); wi != we; ++wi) {
    const uint64_t BeginAddr = Worklist[wi];
    BBInfo *BBI = &BBInfos[BeginAddr];
    MCBasicBlock *&MCBB = BBI->BB;

    MCBB = &MCFN->createBlock(BeginAddr);

    std::swap(MCBB->Insts, BBI->Insts);
    MCBB->InstCount = MCBB->Insts.size();
    MCBB->SizeInBytes = BBI->SizeInBytes;
  }

  // Next, add all predecessors/successors.
  for (size_t wi = 0, we = Worklist.size(); wi != we; ++wi) {
    const uint64_t BeginAddr = Worklist[wi];
    BBInfo *BBI = &BBInfos[BeginAddr];
    MCBasicBlock *&MCBB = BBI->BB;
    RemoveDupsFromAddressVector(BBI->SuccAddrs);
    for (uint64_t Address : BBI->SuccAddrs) {
      MCBasicBlock *Succ = BBInfos[Address].BB;
      assert(Succ && "Couldn't find block successor?!");
      // FIXME: Sort the succs/preds at the end?
      MCBB->Successors.push_back(Succ);
      Succ->Predecessors.push_back(MCBB);
    }
  }
}

MCFunction *
MCObjectDisassembler::createFunction(MCModule *Module, uint64_t BeginAddr,
                                     AddressSetTy &CallTargets,
                                     AddressSetTy &TailCallTargets) {
  AddrPrettyStackTraceEntry X(BeginAddr, "Function");

  // First, check if this is an external function.
  StringRef ExtFnName;
  if (MOS)
    ExtFnName = MOS->findExternalFunctionAt(BeginAddr);
  if (!ExtFnName.empty())
    return Module->createFunction(ExtFnName, BeginAddr);

  // If it's not, look for an existing function.
  if (MCFunction *Fn = Module->findFunctionAt(BeginAddr))
    return Fn;

  // Finally, just create a new one.
  MCFunction *MCFN =
      Module->createFunction(("fn_" + utohexstr(BeginAddr)).c_str(), BeginAddr);
  disassembleFunctionAt(Module, MCFN, BeginAddr, CallTargets, TailCallTargets);
  return MCFN;
}

llvm::MCObjectDisassembler::AddressSetTy MCObjectDisassembler::findFunctionStarts() {
    AddressSetTy Starts;

    MachOObjectFile *MachO = dyn_cast<MachOObjectFile>((ObjectFile*)&Obj);

    assert(MachO && "Handling Stripped Binaries is only handled for Mach-O");

    MachOObjectFile::load_command_iterator Load_it = MachO->begin_load_commands();
    MachOObjectFile::load_command_iterator Load_it_end = MachO->end_load_commands();
    for(; Load_it != Load_it_end; ++Load_it) {
        if (Load_it->C.cmd == MachO::LC_FUNCTION_STARTS) {
            uint32_t *Cmd_ptr = (uint32_t*) Load_it->Ptr;
            uint32_t Offset = Cmd_ptr[2];
            uint32_t Size = Cmd_ptr[3];
            MachO::linkedit_data_command c = MachO->getLinkeditDataLoadCommand(*Load_it);
            errs() << c.dataoff << "\n" << c.datasize << "\n";
            StringRef FunctionStarts = MachO->getData().slice(Offset, Offset + Size);
//            MemoryBuffer buffer = MemoryBuffer::getMemBuffer(FunctionStarts);
            uint64_t LastFunctionStart = 0x0;
            for (unsigned i = 0; i < Size;) {
                unsigned n = 0;
                char *p = (char*) &FunctionStarts.data()[i];
                uint64_t V = decodeULEB128((uint8_t*)p, &n);
                assert(n);
                LastFunctionStart += V;
                Starts.push_back(LastFunctionStart | 0x100000000);
                i += n;
            }
        }
    }

//    for (auto &f : Starts) {
//        errs() << utohexstr(f) << "\n";
//    }

    std::sort(Starts.begin(), Starts.end());

    return Starts;
}

bool MCObjectDisassembler::checkBranch(MCInst &Inst, uint64_t Target) {
    if (!ObjCFile)
        return false;
    std::string FunctionName = ObjCFile->getFunctionName(Target);
    for (object::section_iterator S_it = Obj.section_begin(); S_it != Obj.section_end(); ++S_it) {
        StringRef SectionName;
        if (!S_it->getName(SectionName)) {
            if (SectionName == "__stubs") {
                if (S_it->getAddress() <= Target && Target < (S_it->getAddress() + S_it->getSize())) {
                    return true;
                }
            }
        }
    }
    return false;
}
