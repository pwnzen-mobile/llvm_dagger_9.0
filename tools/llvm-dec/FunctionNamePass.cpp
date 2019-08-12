#include <llvm/IR/Module.h>
#include "FunctionNamePass.h"
#include <system_error>
#include <llvm/MC/MCInst.h>
#include "llvm/Support/LEB128.h"
#include "llvm/Support/Debug.h"
#include <sstream>
#include <llvm/ADT/StringExtras.h>
#include "llvm/Analysis/CallGraph.h"
#include <vector>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Instructions.h>
#include "llvm/Object/ObjectiveCFile.h"
#include "llvm-c/Core.h"

#define DEBUG_TYPE "func_name_pass"

using namespace llvm;
using namespace object;

enum AArch64Instruction {
    NOP = 738,
    LDR = 1107,
    LDRX = 1112,
    BR = 127,
    ADRP = 83
};

static char ID;
FunctionNamePass::FunctionNamePass(object::MachOObjectFile *MachO, std::unique_ptr<MCDisassembler> &DisAsm) :
        ModulePass(ID), MachO(MachO), DisAsm(DisAsm) {
    resolveSymbols();

    ObjectiveCFile C(MachO);
    FunctionNamesMap_t ObjectiveCFunctionNames = C.getFunctionNames();
    FunctionNames.insert(ObjectiveCFunctionNames.begin(), ObjectiveCFunctionNames.end());
}

bool FunctionNamePass::runOnModule(Module &M) {

    std::vector<std::string> FunctionsToReplace;
    for (StubToLocalMap_t::iterator it = StubToLocal.begin(); it != StubToLocal.end(); ++it) {
        FunctionsToReplace.push_back("fn_" + utohexstr(it->first));
        DEBUG(errs() << "Replace calls to: fn_" <<utohexstr(it->first) << "\n");
    }



    CallGraph CG(M);
    //for (Module::FunctionListType::iterator F_it = M.getFunctionList().begin();
      //   F_it != M.getFunctionList().end(); ++F_it) {
             //for()
        const auto &FL = M.getFunctionList();
        for (const Function &Fn : FL) {    
       // CallGraphNode *CGN = CG.getOrInsertFunction(F_it);
        CallGraphNode *CGN = CG.getOrInsertFunction(&Fn);
        for (CallGraphNode::iterator i = CGN->begin(); i != CGN->end(); ++i) {
            if (!i->second->getFunction())
                continue;
            std::vector<std::string>::iterator X = std::find(FunctionsToReplace.begin(), FunctionsToReplace.end(), i->second->getFunction()->getName().str());
            if (X != FunctionsToReplace.end()) {
                Value &V = *i->first;
                if (CallInst *Call = dyn_cast<CallInst>(&V)) {
                    StringRef FName = Call->getCalledFunction()->getName().substr(3);

                    std::stringstream ss;
                    ss << std::hex << FName.str();
                    uint64_t Addr;
                    ss >> Addr;
                    if (Addr) {
                        uint64_t R = StubToLocal[Addr];
                        StringRef RName = "fn_" + utohexstr(R);
                        //DEBUG(errs() << "Replace " << Call->getCalledFunction()->getName() << " with " << RName << "\n");
                        Function *F = M.getFunction(RName);
                        assert(F);
                        Call->setCalledFunction(F);
                    }
                } else {
                    llvm_unreachable("This should not happen...");
                }

            }
        }
    }


    for (Module::FunctionListType::iterator F_it = M.getFunctionList().begin();
            F_it != M.getFunctionList().end(); ++F_it) {
        StringRef Name = F_it->getName();
        StringRef Address = Name.substr(3);
        uint64_t A;

        //TODO: Maybe use a StringRef function here...
        std::stringstream ss;
        ss << std::hex << Address.str();
        ss >> A;

        if (A) {
            if (FunctionNames.find(A) != FunctionNames.end()) {
                DEBUG(errs() << "Change fn_" << utohexstr(A) << " to " << FunctionNames[A] << "\n");
                F_it->setName(FunctionNames[A]);

            }
        } else {
            errs() << "Can't convert int: " << Address << "\n";
        }
    }


    for (Module::iterator F_it = M.begin(); F_it != M.end(); ++F_it) {
        for (Function::iterator BB_it = F_it->begin(); BB_it != F_it->end(); ++BB_it) {
            StringRef BBName = BB_it->getName().substr(3);
            if (!BBName.endswith("_call"))
                continue;
            BBName = BBName.substr(0, BBName.size() - 5);
            std::stringstream ss;
            ss << std::hex << BBName.str();
            uint64_t Addr;
            ss >> Addr;

            StringRef Name;

            if (Addr) {
                assert(true);
                if (FunctionNames[Addr].size()) {
                    Name = FunctionNames[Addr];
                } else if (StubToLocal[Addr]) {

                }
            }

            if (!Name.size()) {
                //TODO: check what happens here...
                continue;
            }

            for (BasicBlock::iterator I_it = BB_it->begin(); I_it != BB_it->end(); ++I_it) {
                if (I_it->getOpcode() == Instruction::Call) {
                    if (CallInst *Call = dyn_cast<CallInst>(&*I_it)) {
                        if (Call->getCalledFunction() == nullptr) {
                            assert(true);
                            Function *F_Replace = M.getFunction(Name);
                            if (!F_Replace) {
                              //  F_Replace = dyn_cast<Function>(M.getOrInsertFunction(Name, Type::getVoidTy(getGlobalContext()), M.getTypeByName("regset")->getPointerTo(),
                               //                       nullptr));
                               F_Replace = dyn_cast<Function>(M.getOrInsertFunction(Name, Type::getVoidTy(getGlobalContext()), M.getTypeByName("regset")->getPointerTo(),
                                                      nullptr).getCallee());
                                assert(F_Replace);
                            }
                            if (!F_Replace->isDeclaration())
                                continue;
                            DEBUG(errs() << "Replace " << BB_it->getName() << " -> " << F_Replace->getName() << "\n");
                            Call->setCalledFunction(F_Replace);
                        }
                    }
                }
            }
        }
    }

    return false;
}

void FunctionNamePass::resolveSymbols() {
    bool hasStubsSection = false;
    SectionRef StubsSection;
    SectionRef StubHelperSection;
    SectionRef LazyPtrSection;
    SectionRef TextSection;

    for (section_iterator i = MachO->section_begin(); i != MachO->section_end(); ++i) {

        StringRef Name;
        i->getName(Name);

        if (Name == "__stubs") {
            StubsSection = *i;
            hasStubsSection = true;
        } else if (Name == "__stub_helper") {
            StubHelperSection = *i;
        } else if (Name == "__la_symbol_ptr") {
            LazyPtrSection = *i;
        } else if (Name == "__text") {
            TextSection = *i;
        }
    }


    if (!hasStubsSection) {
        return;
    }

    StringRef StubsBytesStr;

    StubsSection.getContents(StubsBytesStr);
    uint64_t StubsAddress = StubsSection.getAddress();

    ArrayRef<uint8_t> StubsBytes(reinterpret_cast<const uint8_t *>(StubsBytesStr.data()),
                                 StubsBytesStr.size());

    StringRef LazyPtrBytesStr;
    LazyPtrSection.getContents(LazyPtrBytesStr);
    ArrayRef<uint8_t> LazyPtrBytes(reinterpret_cast<const uint8_t *>(LazyPtrBytesStr.data()),
                                   LazyPtrBytesStr.size());

    StringRef StubHelperBytesStr;
    StubHelperSection.getContents(StubHelperBytesStr);

    ArrayRef<uint8_t> StubHelperBytes(reinterpret_cast<const uint8_t *>(StubHelperBytesStr.data()),
                                      StubHelperBytesStr.size());

    uint64_t TextSectionAddress = TextSection.getAddress();
    uint64_t TextSectionSize = TextSection.getSize();

    ArrayRef<uint8_t> LazyBindInfoOpcodes = MachO->getDyldInfoLazyBindOpcodes();

    uint64_t SectionAddress = StubsSection.getAddress();

    uint64_t StubsSectionSize = StubsSection.getSize();

    for (uint64_t Index = 0; Index < StubsSectionSize; Index += 4) {
        uint64_t  StubAddress = StubsSection.getAddress();
        StubAddress += Index;
        MCInst NOP;
        uint64_t NOPSize;

        if (StubAddress >= 0x1003FF0A4) {
            assert(true);
        }

        if (DisAsm->getInstruction(NOP, NOPSize, StubsBytes.slice(Index),
                                   SectionAddress + Index, nulls(),
                                   nulls()) == 3 ) {

        } else {
            llvm_unreachable("Something went wrong");
        }

        uint64_t LazyPtrAddress = 0;
        if (NOP.getOpcode() == AArch64Instruction::NOP) {

            assert(NOP.getOpcode() == AArch64Instruction::NOP && NOPSize == 4);
            Index += 4;

            MCInst LDR;
            uint64_t LDRSize;
            if (DisAsm->getInstruction(LDR, LDRSize, StubsBytes.slice(Index),
                                       SectionAddress + Index, nulls(),
                                       nulls()) == 3) {

            } else {
                llvm_unreachable("Something went wrong");
            }
            assert(LDR.getOpcode() == AArch64Instruction::LDR && LDRSize == 4);
            Index += 4;

            MCInst BR;
            uint64_t BRSize;
            if (DisAsm->getInstruction(BR, BRSize, StubsBytes.slice(Index),
                                       SectionAddress + Index, nulls(),
                                       nulls()) == 3) {

            } else {
                llvm_unreachable("Something went wrong");
            }

            assert(BR.getOpcode() == AArch64Instruction::BR && BRSize == 4);

            LazyPtrAddress = SectionAddress + Index - 4 + (LDR.getOperand(1).getImm() * 4);
        } else if (NOP.getOpcode() == AArch64Instruction::ADRP) {

            int32_t pageAddress = NOP.getOperand(1).getImm();
            pageAddress = pageAddress & 0x200000 ? pageAddress | 0xFFE00000 : pageAddress;
            uint64_t base = ((SectionAddress + Index) & ~0xFFF) + (pageAddress << 12);

            Index += 4;

            MCInst LDR;
            uint64_t LDRSize;
            if (DisAsm->getInstruction(LDR, LDRSize, StubsBytes.slice(Index),
                                       SectionAddress + Index, nulls(),
                                       nulls()) == 3) {

            } else {
                llvm_unreachable("Something went wrong");
            }
            assert(LDR.getOpcode() == AArch64Instruction::LDRX && LDRSize == 4);
            Index += 4;

            MCInst BR;
            uint64_t BRSize;
            if (DisAsm->getInstruction(BR, BRSize, StubsBytes.slice(Index),
                                       SectionAddress + Index, nulls(),
                                       nulls()) == 3) {

            } else {
                llvm_unreachable("Something went wrong");
            }

            assert(BR.getOpcode() == AArch64Instruction::BR && BRSize == 4);

            LazyPtrAddress = base + (LDR.getOperand(2).getImm() * 8);
        } else {
            assert(false);
        }


        uint64_t LazyPtrSectionAddress = LazyPtrSection.getAddress();
        uint64_t LazyPtrSectionSize = LazyPtrSection.getSize();
        assert(LazyPtrAddress >= LazyPtrSectionAddress && LazyPtrAddress <= LazyPtrSectionAddress + LazyPtrSectionSize);

        uint64_t LazyPtrSectionIndex = LazyPtrAddress - LazyPtrSectionAddress;

        uint64_t LazyPtr = *((uint64_t *)LazyPtrBytes.slice(LazyPtrSectionIndex, 8).data());

        uint64_t StubHelperSectionAddress = StubHelperSection.getAddress();
        uint64_t StubHelperSectionSize = StubHelperSection.getSize();
        if (!(LazyPtr >= StubHelperSectionAddress && LazyPtr <= (StubHelperSectionAddress + StubHelperSectionSize))) {
            if (LazyPtr >= TextSectionAddress && LazyPtr <= (TextSectionAddress + TextSectionSize)) {
                StubToLocal[StubAddress] = LazyPtr;
                DEBUG(errs() << "Stub: " << utohexstr(StubAddress) << " -> " << utohexstr(LazyPtr) << "\n");
            }
            if (LazyPtr == 0) {

                auto getSegment = [&](uint64_t SegmentNo) {
                    uint64_t Idx = 0;
                    MachO::segment_command_64 segment;
                    for(object::MachOObjectFile::load_command_iterator L_it = MachO->begin_load_commands();
                        L_it != MachO->end_load_commands(); ++L_it) {
                        if (L_it->C.cmd == MachO::LC_SEGMENT_64) {
                            if (Idx == SegmentNo) {
                                segment = MachO->getSegment64LoadCommand(*L_it);
                                break;
                            } else {
                                Idx++;
                            }
                        }
                    }
                    return segment;
                };

                ArrayRef<uint8_t> BindOpcodes = MachO->getDyldInfoWeakBindOpcodes();

                uint64_t SegmentNo = 0;
                int64_t Offset = 0;
                StringRef SymbolName;

                bool print = false;

                for (unsigned Idx = 0; Idx < BindOpcodes.size();) {
                    unsigned Opcode = *BindOpcodes.slice(Idx).data();

                    if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_DYLIB_ORDINAL_IMM) {
                        unsigned Imm = Opcode & 0xF;
                        Idx++;
                    } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM) {
                        Idx++;
                        SymbolName = (const char*)BindOpcodes.slice(Idx).data();
                        Idx += SymbolName.size() + 1;
                    } else if (Opcode == (MachO::BIND_OPCODE_SET_TYPE_IMM | 0x1)) {
                        //Set type pointer
                        Idx++;
                    } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB) {
                        SegmentNo = Opcode & 0xF;
                        Idx++;
                        unsigned n;
                        Offset = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
                        Idx += n;
                    } else if (Opcode == MachO::BIND_OPCODE_DO_BIND) {
                        MachO::segment_command_64 Segment = getSegment(SegmentNo);

                        if (print) {
                            errs() << utohexstr(Segment.vmaddr + Offset) << "\n";
                        }
                        if (Segment.vmaddr + Offset == LazyPtrAddress) {
                            FunctionNames[StubAddress] = SymbolName.substr(1);
                            break;
                        }
                        //FIXME: is this add correct???
                        Offset += 8;
                        Idx++;
                    } else if (Opcode == MachO::BIND_OPCODE_ADD_ADDR_ULEB) {
                        Idx++;
                        unsigned n;
                        Offset += decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
                        Idx += n;
                    } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED) {
                        unsigned Scaled = (Opcode & 0xF) * 8;
                        //FIXME: again the add of 8
                        Offset += Scaled + 8;
                        Idx++;
                    } else if (Opcode == MachO::BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB) {
                        //TODO:
                        Idx++;
                        unsigned n;
                        uint64_t Count = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
                        Idx += n;
                        n = 0;
                        uint64_t Skip = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
                        for (unsigned i = 0; i < Count; ++i) {
                            Offset += 8 + Skip;
                        }
                        Idx += n;
                    } else if (Opcode == MachO::BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB) {
                        Idx++;
                        unsigned n;
                        Offset += decodeULEB128(BindOpcodes.slice(Idx).data(), &n) + 8;
                        Idx += n;
                    } else if (Opcode == MachO::BIND_OPCODE_DONE) {
                        break;
                    } else if (Opcode == MachO::BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB) {
                        Idx++;
                        unsigned n;
                        uint64_t dylib = decodeULEB128(BindOpcodes.slice(Idx).data(), &n) + 8;
                        Idx += n;
                    } else if (Opcode == MachO::BIND_OPCODE_SET_ADDEND_SLEB) {
                        Idx++;
                        unsigned n;
                        uint64_t addend = decodeSLEB128(BindOpcodes.slice(Idx).data(), &n);
                        Idx += n;
                    } else {
                        errs() << "0X" << utohexstr(Idx) << "\n";
                        errs() << "0x" << utohexstr(Opcode) << "\n";
                        assert(false);
                    }
                }
            }
            continue;
        }

        uint64_t LazyPtrIndex = StubHelperSection.getAddress();
        LazyPtrIndex = LazyPtr - LazyPtrIndex;

        MCInst LazyLDR;
        uint64_t LazyLDRSize;
        if (DisAsm->getInstruction(LazyLDR, LazyLDRSize, StubHelperBytes.slice(LazyPtrIndex),
                                   StubHelperSectionAddress + LazyPtrIndex, nulls(),
                                   nulls()) == 3 ) {

        } else {
            llvm_unreachable("Something went wrong");
        }
        int64_t Offset = LazyLDR.getOperand(1).getImm() * 4;
        uint32_t ID = *((uint32_t *)StubHelperBytes.slice(LazyPtrIndex + Offset).data());

        const uint8_t *LazyBindInfoElement = LazyBindInfoOpcodes.slice(ID).data();
        StringRef SymbolName;
        while (1) {
            int8_t  Val = *LazyBindInfoElement;
            if ((Val & 0x70) == 0x70) {
                LazyBindInfoElement++;
                unsigned n = 0;
                decodeULEB128(LazyBindInfoElement, &n);
                LazyBindInfoElement += n;
            } else if ((Val & 0x10) == 0x10) {
                LazyBindInfoElement++;
            } else if ((Val & 0x20) == 0x20) {
                LazyBindInfoElement++;
                unsigned n = 0;
                decodeULEB128(LazyBindInfoElement, &n);
                LazyBindInfoElement += n;
            } else if ((Val & 0x40) == 0x40) {
                LazyBindInfoElement++;
                SymbolName = StringRef((char*)LazyBindInfoElement);
                break;
            } else {
                errs().write_hex(Val);
                llvm_unreachable("");
            }
        }
        DEBUG(errs() << "Resolved Symbol \""<< SymbolName << "\": ");
        DEBUG(errs().write_hex(StubAddress));
        DEBUG(errs() << "\n");
        //Skipe the first '_'
        FunctionNames[StubAddress] = SymbolName.substr(1);
    }
}
