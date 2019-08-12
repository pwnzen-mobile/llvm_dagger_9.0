#ifndef LLVM_TAILCALLPASS_H
#define LLVM_TAILCALLPASS_H

#include <llvm/MC/MCDisassembler/MCDisassembler.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Object/MachO.h"
#include "llvm/MC/MCObjectDisassembler.h"
#include <map>

namespace llvm {

    class TailCallPass : public ModulePass {
    public:
        TailCallPass(MCObjectDisassembler::AddressSetTy functionStarts);
        virtual bool runOnModule(Module &M) override;

    private:
        MCObjectDisassembler::AddressSetTy functionStarts;
    };
}

#endif //LLVM_TAILCALLPASS_H
