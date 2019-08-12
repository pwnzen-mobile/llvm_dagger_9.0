#include "TailCallPass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"

#include <sstream>
#include <llvm/ADT/StringExtras.h>
#include <set>

using namespace llvm;

static char ID;

TailCallPass::TailCallPass(MCObjectDisassembler::AddressSetTy functionStarts) : ModulePass(ID) {
    this->functionStarts = functionStarts;

    std::sort(this->functionStarts.begin(), this->functionStarts.end());
}

bool TailCallPass::runOnModule(Module &M) {

    auto getFunctionAddress = [&](StringRef name) {
        uint64_t addr = 0;
        if (name.size() != 12) {
        } else {

            StringRef addrStr = name.substr(name.size() - 9);

            if (addrStr.size() == 9) {
                std::stringstream ss;
                ss << std::hex << addrStr.str();
                ss >> addr;
            }
        }
        return addr;
    };

    for (auto &function : M.functions()) {

        if (function.isDeclaration() || function.isIntrinsic())
            continue;

        if (!function.getName().startswith("fn_")) {
            continue;
        }

        uint64_t functionAddr = getFunctionAddress(function.getName());

        typedef MCObjectDisassembler::AddressSetTy FunctionStart_t;
        FunctionStart_t::iterator startIt = std::find(functionStarts.begin(), functionStarts.end(), functionAddr);
        FunctionStart_t::iterator endIt = startIt;
        std::advance(endIt, 1);

        uint64_t startAddr = *startIt;
        uint64_t endAddr = (endIt == functionStarts.end()) ? UINT64_MAX : *endIt;

        BasicBlock *exitBB = nullptr;
        ReturnInst *retInst = nullptr;

        for (auto &bb : function) {
            if (Instruction *term = bb.getTerminator()) {
                if (ReturnInst *ret = dyn_cast<ReturnInst>(term)) {
                    exitBB = ret->getParent();
                    retInst = ret;
                }
            }
        }

        if (!exitBB || !retInst) {
            continue;
        }

        for (auto &bb : function) {
            std::set<Instruction*> remove;
            for (auto &inst : bb) {
                if (UnreachableInst *unreachable = dyn_cast<UnreachableInst>(&inst)) {
                    for (auto &i : bb) {
                        remove.insert(&i);
                    }
                    break;
                }
            }

            for (auto &r : remove) {
                r->dropAllReferences();
                r->removeFromParent();
            }

            IRBuilder<> builder(&bb);
            builder.CreateBr(exitBB);
        }

//        for (auto &bb : function) {
//            if (TerminatorInst *term = bb.getTerminator()) {
//                if (BranchInst *br = dyn_cast<BranchInst>(term)) {
//                    if (br->getNumOperands() > 1) {
//                        continue;
//                    }
//                    if (BasicBlock *branchTarget = dyn_cast<BasicBlock>(br->getOperand(0))) {
//                        std::string bbName = branchTarget->getName();
//                        uint64_t bbAddr = getFunctionAddress(bbName);
//                        if (bbAddr && !(startAddr <= bbAddr && bbAddr <= endAddr)) {
//                            assert(true);
//                            std::string targetFunctionName = "fn_" + utohexstr(bbAddr, false);
//                            Function *targetFunction = M.getFunction(targetFunctionName);
//                            if (targetFunction) {
//                                IRBuilder<> builder(term);
//
////                                builder.CreateBr(exitBB);
////                                term->dropAllReferences();
////                                term->removeFromParent();
//
////                                builder.SetInsertPoint(retInst);
////                                builder.CreateCall(targetFunction, {&function.getArgumentList().front()});
//
//
//                                BasicBlock *newBB = BasicBlock::Create(getGlobalContext(), "foobar", &function);
//
//                                term->setOperand(0, newBB);
//
//                                builder.SetInsertPoint(newBB);
//
//                                for (auto &entryInst : function.getEntryBlock()){
//                                    if (entryInst.getOpcode() == Instruction::GetElementPtr) {
//                                        Value *load = builder.CreateLoad(&entryInst);
//                                        builder.CreateStore(load, &entryInst);
//                                    }
//                                }
//
//                                builder.CreateCall(targetFunction, {&function.getArgumentList().front()});
//
//                                builder.CreateBr(exitBB);
//
//                            } else {
//                                continue;
//                            }
//
//                        }
//                    }
//                }
//            }
//        }
    }
    return false;
}
