#ifndef LLVM_LIB_TARGET_AARCH64_DC_AARCH64INSTRSEMA_H
#define LLVM_LIB_TARGET_AARCH64_DC_AARCH64INSTRSEMA_H

#include "llvm/DC/DCInstrSema.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

class AArch64InstrSema : public DCInstrSema {

public:
  AArch64InstrSema(DCRegisterSema &DRS);

  virtual void translateTargetOpcode();
  virtual void translateCustomOperand(unsigned OperandType,
                                      unsigned MIOperandNo);
  virtual void translateImplicit(unsigned RegNo) {};
protected:
    virtual bool translateTargetInst() override;
    virtual void translateTargetIntrinsic(unsigned IntrinsicID);

private:
    void printInstruction();

    Value *getNZCVFlags(Value *Result, Value *LHS = NULL, Value *RHS = NULL);
    Value *getNZCVFlag(Value *N, Value *Z, Value *C = NULL, Value *V = NULL);

    Value *ArithExtend(Value *Value, Type *ExtType, uint64_t Ext);
    Value *FPCompare(Value *LHS, Value *RHS);
};
}

#endif
