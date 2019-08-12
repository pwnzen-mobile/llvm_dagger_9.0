#ifndef LLVM_LIB_TARGET_AARCH64_DC_AARCH64REGISTERSEMA_H
#define LLVM_LIB_TARGET_AARCH64_DC_AARCH64REGISTERSEMA_H

#include "AArch64InstrInfo.h"
#include "llvm/DC/DCRegisterSema.h"
#include "llvm/ADT/SmallVector.h"

namespace llvm {
    class AArch64RegisterSema : public DCRegisterSema {
    public:
      AArch64RegisterSema(const MCRegisterInfo &MRI,
                      const MCInstrInfo &MII,
                      const DataLayout &DL);

      virtual void insertInitRegSetCode(Function *InitFn);
      virtual void insertFiniRegSetCode(Function *FiniFn);

        virtual Type *getRegType(unsigned RegNo);

    protected:
        virtual bool doesSubRegIndexClearSuper(unsigned Idx) const override;

        virtual void onRegisterSet(unsigned RegNo, Value *Val) override;

        virtual void onRegisterGet(unsigned RegNo) override;

    public:
        virtual Value *getReg(unsigned RegNo) override;

        virtual void setReg(unsigned RegNo, Value *Val) override;
    };
}

#endif
