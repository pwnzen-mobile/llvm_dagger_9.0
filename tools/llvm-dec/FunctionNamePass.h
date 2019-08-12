#ifndef LLVM_FUNCTIONNAMEPASS_H
#define LLVM_FUNCTIONNAMEPASS_H

#include <llvm/MC/MCDisassembler/MCDisassembler.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Object/MachO.h"
#include <map>

namespace llvm {

    class FunctionNamePass : public ModulePass {

    public:
        FunctionNamePass(object::MachOObjectFile *MachO, std::unique_ptr<MCDisassembler> &DisAsm);

        virtual bool runOnModule(Module &M) override;

        typedef std::map<uint64_t, uint64_t> StubToLocalMap_t;
    private:
        typedef std::map<uint64_t, std::string> FunctionNamesMap_t;

        object::MachOObjectFile *MachO;

        void resolveSymbols();
        std::unique_ptr<MCDisassembler> &DisAsm;
        FunctionNamesMap_t FunctionNames;
        StubToLocalMap_t StubToLocal;
    };
}

#endif //LLVM_FUNCTIONNAMEPASS_H
