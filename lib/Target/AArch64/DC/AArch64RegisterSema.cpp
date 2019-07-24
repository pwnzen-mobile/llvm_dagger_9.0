#include <llvm/MC/MCAnalysis/MCFunction.h>
#include <llvm/ADT/StringExtras.h>
#include "AArch64RegisterSema.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "dc-sema"

using namespace llvm;

static void AArch64InitSpecialRegSizes(DCRegisterSema::RegSizeTy &RegSizes) {
//    RegSizes[AArch64::D0] = 128;
}

AArch64RegisterSema::AArch64RegisterSema(const MCRegisterInfo &MRI,
                                         const MCInstrInfo &MII,
                                         const DataLayout &DL) : DCRegisterSema(MRI, MII, DL,
                                                                                AArch64InitSpecialRegSizes) {
                errs()<<"start to create AArch64RegisterSema";
}

Type *AArch64RegisterSema::getRegType(unsigned RegNo) {
  return Type::getInt64Ty(getGlobalContext());
}

void AArch64RegisterSema::insertInitRegSetCode(Function *InitFn) {
  IRBuilderBase::InsertPointGuard IPG(*Builder);
  Type *I64Ty = Builder->getInt64Ty();
  Value *Idx[] = {Builder->getInt32(0), 0};
  Builder->SetInsertPoint(BasicBlock::Create(*Ctx, "", InitFn));

  Function::arg_iterator ArgI = InitFn->getArgumentList().begin();
  Value *RegSet = ArgI++;
  Value *StackPtr = ArgI++;
  Value *StackSize = ArgI++;
  Value *ArgC = ArgI++;
  Value *ArgV = ArgI++;

  Value *RSP = Builder->CreatePtrToInt(StackPtr, I64Ty);
  RSP = Builder->CreateAdd(RSP, Builder->CreateZExtOrBitCast(StackSize, I64Ty));

  // push ~0 to simulate a call
  RSP = Builder->CreateSub(RSP, Builder->getInt64(128));
  Builder->CreateStore(Builder->getInt(APInt::getAllOnesValue(64)),
                       Builder->CreateIntToPtr(RSP, I64Ty->getPointerTo()));

  // put a pointer to the test stack in RSP
  Idx[1] = Builder->getInt32(RegOffsetsInSet[RegLargestSupers[AArch64::SP]]);
  Builder->CreateStore(RSP, Builder->CreateInBoundsGEP(RegSet, Idx));
  Builder->CreateRetVoid();
}

void AArch64RegisterSema::insertFiniRegSetCode(Function *FiniFn) {
  IRBuilderBase::InsertPointGuard IPG(*Builder);
  Value *Idx[] = {Builder->getInt32(0), 0};
  Builder->SetInsertPoint(BasicBlock::Create(*Ctx, "", FiniFn));

  Function::arg_iterator ArgI = FiniFn->getArgumentList().begin();
  Value *RegSet = ArgI;

  // Result comes out of EAX
  Idx[1] = Builder->getInt32(RegOffsetsInSet[RegLargestSupers[AArch64::X0]]);
  Builder->CreateRet(Builder->CreateTrunc(
    Builder->CreateLoad(Builder->CreateInBoundsGEP(RegSet, Idx)),
    Builder->getInt32Ty()));
}

bool AArch64RegisterSema::doesSubRegIndexClearSuper(unsigned Idx) const {
  if (Idx >= AArch64::W0 && Idx <= AArch64::W30) {
    return true;
  }
  if (Idx >= AArch64::D0 && Idx <= AArch64::D31) {
    return true;
  }
  if (Idx >= AArch64::S0 && Idx <= AArch64::S31) {
    return true;
  }
  if (Idx >= AArch64::B0 && Idx <= AArch64::B31) {
    return true;
  }
  return false;
}

void AArch64RegisterSema::onRegisterSet(unsigned RegNo, Value *Val) {
//    if (RegNo == AArch64::XZR) {
//        ConstantInt *Const = dyn_cast<ConstantInt>(Val);
//        if (Const) {
//            if (Const->getValue().getZExtValue() != 0) {
//                setReg(RegNo, Builder->getInt64(0));
//            }
//        } else {
//            setReg(RegNo, Builder->getInt64(0));
//        }
//    } else if (RegNo == AArch64::WZR) {
//        ConstantInt *Const = dyn_cast<ConstantInt>(Val);
//        if (Const) {
//            if (Const->getValue().getZExtValue() != 0) {
//                setReg(RegNo, Builder->getInt32(0));
//            }
//        } else {
//            setReg(RegNo, Builder->getInt32(0));
//        }
//    }
}


void AArch64RegisterSema::onRegisterGet(unsigned RegNo) {
//    if (RegNo == AArch64::XZR) {
//        RegVals[RegNo] = Builder->getInt64(0);
//    }
//    if (RegNo == AArch64::WZR) {
//        RegVals[RegNo] = Builder->getInt32(0);
//    }
}

Value *AArch64RegisterSema::getReg(unsigned RegNo) {
  if (RegNo == AArch64::WZR) {
    return Builder->getInt32(0);
  }
  if (RegNo == AArch64::XZR) {
    return Builder->getInt64(0);
  }
  if (RegNo >= AArch64::Q0 && RegNo <= AArch64::Q31) {
    if (!RegVals[RegNo]) {
      Value *QQQQ = getRegNoCallback(RegNo + (AArch64::Q0_Q1_Q2_Q3 - AArch64::Q0));
      Value *Q = Builder->CreateTrunc(QQQQ, Builder->getInt128Ty());
      RegVals[RegNo] = Q;
      return RegVals[RegNo];
    }
  }
  if (RegNo >= AArch64::D0 && RegNo <= AArch64::D31) {
    if (!RegVals[RegNo]) {
      Value *QQQQ = getRegNoCallback(RegNo + (AArch64::Q0_Q1_Q2_Q3 - AArch64::D0));
      Value *D = Builder->CreateTrunc(QQQQ, Builder->getInt64Ty());
      RegVals[RegNo] = D;
      return RegVals[RegNo];
    }
  }
  if (RegNo >= AArch64::S0 && RegNo <= AArch64::S31) {
    if (!RegVals[RegNo]) {
      Value *QQQQ = getRegNoCallback(RegNo + (AArch64::Q0_Q1_Q2_Q3 - AArch64::S0));
      Value *S = Builder->CreateTrunc(QQQQ, Builder->getInt32Ty());
      RegVals[RegNo] = S;
      return RegVals[RegNo];
    }
  }
  if (RegNo >= AArch64::H0 && RegNo <= AArch64::H31) {
    if (!RegVals[RegNo]) {
      Value *QQQQ = getRegNoCallback(RegNo + (AArch64::Q0_Q1_Q2_Q3 - AArch64::H0));
      Value *H = Builder->CreateTrunc(QQQQ, Builder->getInt16Ty());
      RegVals[RegNo] = H;
      return RegVals[RegNo];
    }
  }
  if (RegNo >= AArch64::B0 && RegNo <= AArch64::B31) {
    if (!RegVals[RegNo]) {
      Value *QQQQ = getRegNoCallback(RegNo + (AArch64::Q0_Q1_Q2_Q3 - AArch64::B0));
      Value *B = Builder->CreateTrunc(QQQQ, Builder->getInt8Ty());
      RegVals[RegNo] = B;
      return RegVals[RegNo];
    }
  }
  if (RegNo >= AArch64::Q0_Q1 && RegNo <= AArch64::Q31_Q0) {
    int64_t diff1 = (RegNo - AArch64::Q0_Q1) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1 + 1) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);

    Reg1 = Builder->CreateZExt(Reg1, IntegerType::get(getGlobalContext(), 256));
    Reg2 = Builder->CreateZExt(Reg2, IntegerType::get(getGlobalContext(), 256));
    Reg2 = Builder->CreateShl(Reg2, 128);

    return Builder->CreateOr(Reg1, Reg2);
  }

  if (RegNo >= AArch64::D0_D1 && RegNo <= AArch64::D31_D0) {
    int64_t diff1 = (RegNo - AArch64::D0_D1) % 32;
    int64_t diff2 = (RegNo - AArch64::D0_D1 + 1) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);

    Reg1 = Builder->CreateZExtOrTrunc(Reg1, IntegerType::get(getGlobalContext(), 128));
    Reg2 = Builder->CreateZExtOrTrunc(Reg2, IntegerType::get(getGlobalContext(), 128));
    Reg2 = Builder->CreateShl(Reg2, 64);

    return Builder->CreateOr(Reg1, Reg2);
  }

  if (RegNo >= AArch64::Q0_Q1_Q2 && RegNo <= AArch64::Q31_Q0_Q1) {
    int64_t diff1 = (RegNo - AArch64::Q0_Q1_Q2) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1_Q2 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::Q0_Q1_Q2 + 2) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);
    Value *Reg3 = getReg(AArch64::Q0 + diff3);

    Reg1 = Builder->CreateZExt(Reg1, IntegerType::get(getGlobalContext(), 384));
    Reg2 = Builder->CreateZExt(Reg2, IntegerType::get(getGlobalContext(), 384));
    Reg2 = Builder->CreateShl(Reg2, 128);
    Reg3 = Builder->CreateZExt(Reg3, IntegerType::get(getGlobalContext(), 384));
    Reg3 = Builder->CreateShl(Reg3, 256);

    return Builder->CreateOr(Reg1, Builder->CreateOr(Reg2, Reg3));
  }
  if (RegNo >= AArch64::Q0_Q1_Q2 && RegNo <= AArch64::Q31_Q0_Q1) {
    int64_t diff1 = (RegNo - AArch64::Q0_Q1_Q2) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1_Q2 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::Q0_Q1_Q2 + 2) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);
    Value *Reg3 = getReg(AArch64::Q0 + diff3);

    Reg1 = Builder->CreateZExt(Reg1, IntegerType::get(getGlobalContext(), 384));
    Reg2 = Builder->CreateZExt(Reg2, IntegerType::get(getGlobalContext(), 384));
    Reg2 = Builder->CreateShl(Reg2, 128);
    Reg3 = Builder->CreateZExt(Reg3, IntegerType::get(getGlobalContext(), 384));
    Reg3 = Builder->CreateShl(Reg3, 256);

    return Builder->CreateOr(Reg1, Builder->CreateOr(Reg2, Reg3));
  }

  if (RegNo >= AArch64::D0_D1_D2 && RegNo <= AArch64::D31_D0_D1) {
    int64_t diff1 = (RegNo - AArch64::D0_D1_D2) % 32;
    int64_t diff2 = (RegNo - AArch64::D0_D1_D2 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::D0_D1_D2 + 2) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);
    Value *Reg3 = getReg(AArch64::Q0 + diff3);

    Reg1 = Builder->CreateZExt(Reg1, IntegerType::get(getGlobalContext(), 192));
    Reg2 = Builder->CreateZExt(Reg2, IntegerType::get(getGlobalContext(), 192));
    Reg2 = Builder->CreateShl(Reg2, 64);
    Reg3 = Builder->CreateZExt(Reg3, IntegerType::get(getGlobalContext(), 192));
    Reg3 = Builder->CreateShl(Reg3, 128);

    return Builder->CreateOr(Reg1, Builder->CreateOr(Reg2, Reg3));
  }

  if (RegNo >= AArch64::Q0_Q1_Q2_Q3 && RegNo <= AArch64::Q31_Q0_Q1_Q2) {
    int64_t diff1 = (RegNo - AArch64::Q0_Q1_Q2_Q3) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1_Q2_Q3 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::Q0_Q1_Q2_Q3 + 2) % 32;
    int64_t diff4 = (RegNo - AArch64::Q0_Q1_Q2_Q3 + 3) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);
    Value *Reg3 = getReg(AArch64::Q0 + diff3);
    Value *Reg4 = getReg(AArch64::Q0 + diff4);

    Reg1 = Builder->CreateZExt(Reg1, IntegerType::get(getGlobalContext(), 512));
    Reg2 = Builder->CreateZExt(Reg2, IntegerType::get(getGlobalContext(), 512));
    Reg2 = Builder->CreateShl(Reg2, 128);
    Reg3 = Builder->CreateZExt(Reg3, IntegerType::get(getGlobalContext(), 512));
    Reg3 = Builder->CreateShl(Reg3, 256);
    Reg4 = Builder->CreateZExt(Reg4, IntegerType::get(getGlobalContext(), 512));
    Reg4 = Builder->CreateShl(Reg4, 384);

    return Builder->CreateOr(Reg1, Builder->CreateOr(Reg2, Builder->CreateOr(Reg3, Reg4)));
  }

  if (RegNo >= AArch64::D0_D1_D2_D3 && RegNo <= AArch64::D31_D0_D1_D2) {
    int64_t diff1 = (RegNo - AArch64::D0_D1_D2_D3) % 32;
    int64_t diff2 = (RegNo - AArch64::D0_D1_D2_D3 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::D0_D1_D2_D3 + 2) % 32;
    int64_t diff4 = (RegNo - AArch64::D0_D1_D2_D3 + 3) % 32;

    Value *Reg1 = getReg(AArch64::Q0 + diff1);
    Value *Reg2 = getReg(AArch64::Q0 + diff2);
    Value *Reg3 = getReg(AArch64::Q0 + diff3);
    Value *Reg4 = getReg(AArch64::Q0 + diff4);

    Reg1 = Builder->CreateZExt(Reg1, IntegerType::get(getGlobalContext(), 256));
    Reg2 = Builder->CreateZExt(Reg2, IntegerType::get(getGlobalContext(), 256));
    Reg2 = Builder->CreateShl(Reg2, 64);
    Reg3 = Builder->CreateZExt(Reg3, IntegerType::get(getGlobalContext(), 256));
    Reg3 = Builder->CreateShl(Reg3, 128);
    Reg4 = Builder->CreateZExt(Reg4, IntegerType::get(getGlobalContext(), 256));
    Reg4 = Builder->CreateShl(Reg4, 192);

    return Builder->CreateOr(Reg1, Builder->CreateOr(Reg2, Builder->CreateOr(Reg3, Reg4)));
  }
  return DCRegisterSema::getReg(RegNo);
}

void AArch64RegisterSema::setReg(unsigned RegNo, Value *Val) {
  if (RegNo == 0) {
    //FIXME: possibly PC
    DEBUG(errs() << utohexstr(CurrentInst->Address) << ": Register 0 set (PC???)\n");
    return;
  }
  if (RegNo == AArch64::WZR) {
    return;
  }
  if (RegNo == AArch64::XZR) {
    return;
  }
  if (RegNo >= AArch64::Q0 && RegNo <= AArch64::Q31) {
    setRegNoSubSuper(RegNo, Val);
    setRegNoSubSuper(RegNo - (AArch64::Q0 - AArch64::Q0_Q1_Q2_Q3),
                     Builder->CreateZExt(Val, IntegerType::get(*Ctx, 512)));
    return;
  }
  if (RegNo >= AArch64::D0 && RegNo <= AArch64::D31) {
    setRegNoSubSuper(RegNo, Val);
    setRegNoSubSuper(RegNo - (AArch64::D0 - AArch64::Q0_Q1_Q2_Q3),
                     Builder->CreateZExt(Val, IntegerType::get(*Ctx, 512)));
    return;
  }
  if (RegNo >= AArch64::B0 && RegNo <= AArch64::B31) {
    setRegNoSubSuper(RegNo, Val);
    setRegNoSubSuper(RegNo - (AArch64::B0 - AArch64::Q0_Q1_Q2_Q3),
                     Builder->CreateZExt(Val, IntegerType::get(*Ctx, 512)));
    return;
  }
  if (RegNo >= AArch64::H0 && RegNo <= AArch64::H31) {
    setRegNoSubSuper(RegNo, Val);
    setRegNoSubSuper(RegNo - (AArch64::H0 - AArch64::Q0_Q1_Q2_Q3),
                     Builder->CreateZExt(Val, IntegerType::get(*Ctx, 512)));
    return;
  }
  if (RegNo >= AArch64::S0 && RegNo <= AArch64::S31) {
    setRegNoSubSuper(RegNo, Val);
    setRegNoSubSuper(RegNo - (AArch64::S0 - AArch64::Q0_Q1_Q2_Q3),
                     Builder->CreateZExt(Val, IntegerType::get(*Ctx, 512)));
    return;
  }
  if (RegNo >= AArch64::Q0_Q1 && RegNo <= AArch64::Q31_Q0) {
    Value *reg1 = Builder->CreateTrunc(Val, Builder->getInt128Ty());
    Value *reg2 = Builder->CreateLShr(Val, 128);
    reg2 = Builder->CreateTrunc(reg2, Builder->getInt128Ty());

    int64_t diff1 = (RegNo - AArch64::Q0_Q1) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1 + 1) % 32;

    setReg(AArch64::Q0 + diff1, reg1);
    setReg(AArch64::Q0 + diff2, reg2);

    return;
  }

  if (RegNo >= AArch64::D0_D1 && RegNo <= AArch64::D31_D0) {
    Value *reg1 = Builder->CreateTrunc(Val, Builder->getInt64Ty());
    Value *reg2 = Builder->CreateLShr(Val, 64);
    reg2 = Builder->CreateTrunc(reg2, Builder->getInt64Ty());

    int64_t diff1 = (RegNo - AArch64::D0_D1) % 32;
    int64_t diff2 = (RegNo - AArch64::D0_D1 + 1) % 32;

    setReg(AArch64::D0 + diff1, reg1);
    setReg(AArch64::D0 + diff2, reg2);

    return;
  }

  if (RegNo >= AArch64::Q0_Q1_Q2 && RegNo <= AArch64::Q31_Q0_Q1) {
    Value *reg1 = Builder->CreateTrunc(Val, Builder->getInt128Ty());
    Value *reg2 = Builder->CreateLShr(Val, 128);
    reg2 = Builder->CreateTrunc(reg2, Builder->getInt128Ty());
    Value *reg3 = Builder->CreateLShr(Val, 256);
    reg3 = Builder->CreateTrunc(reg3, Builder->getInt128Ty());

    int64_t diff1 = (RegNo - AArch64::Q0_Q1_Q2) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1_Q2 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::Q0_Q1_Q2 + 2) % 32;

    setReg(AArch64::Q0 + diff1, reg1);
    setReg(AArch64::Q0 + diff2, reg2);
    setReg(AArch64::Q0 + diff3, reg3);

    return;
  }

  if (RegNo >= AArch64::D0_D1_D2 && RegNo <= AArch64::D31_D0_D1) {
    Value *reg1 = Builder->CreateTrunc(Val, Builder->getInt64Ty());
    Value *reg2 = Builder->CreateLShr(Val, 64);
    reg2 = Builder->CreateTrunc(reg2, Builder->getInt64Ty());
    Value *reg3 = Builder->CreateLShr(Val, 128);
    reg3 = Builder->CreateTrunc(reg3, Builder->getInt64Ty());

    int64_t diff1 = (RegNo - AArch64::D0_D1_D2) % 32;
    int64_t diff2 = (RegNo - AArch64::D0_D1_D2 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::D0_D1_D2 + 2) % 32;

    setReg(AArch64::D0 + diff1, reg1);
    setReg(AArch64::D0 + diff2, reg2);
    setReg(AArch64::D0 + diff3, reg3);

    return;
  }

  if (RegNo >= AArch64::Q0_Q1_Q2_Q3 && RegNo <= AArch64::Q31_Q0_Q1_Q2) {
    Value *reg1 = Builder->CreateTrunc(Val, Builder->getInt128Ty());
    Value *reg2 = Builder->CreateLShr(Val, 128);
    reg2 = Builder->CreateTrunc(reg2, Builder->getInt128Ty());
    Value *reg3 = Builder->CreateLShr(Val, 256);
    reg3 = Builder->CreateTrunc(reg3, Builder->getInt128Ty());
    Value *reg4 = Builder->CreateLShr(Val, 384);
    reg4 = Builder->CreateTrunc(reg4, Builder->getInt128Ty());

    int64_t diff1 = (RegNo - AArch64::Q0_Q1_Q2_Q3) % 32;
    int64_t diff2 = (RegNo - AArch64::Q0_Q1_Q2_Q3 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::Q0_Q1_Q2_Q3 + 2) % 32;
    int64_t diff4 = (RegNo - AArch64::Q0_Q1_Q2_Q3 + 3) % 32;

    setReg(AArch64::Q0 + diff1, reg1);
    setReg(AArch64::Q0 + diff2, reg2);
    setReg(AArch64::Q0 + diff3, reg3);
    setReg(AArch64::Q0 + diff4, reg4);

    return;
  }

  if (RegNo >= AArch64::D0_D1_D2_D3 && RegNo <= AArch64::D31_D0_D1_D2) {
    Value *reg1 = Builder->CreateTrunc(Val, Builder->getInt64Ty());
    Value *reg2 = Builder->CreateLShr(Val, 64);
    reg2 = Builder->CreateTrunc(reg2, Builder->getInt64Ty());
    Value *reg3 = Builder->CreateLShr(Val, 128);
    reg3 = Builder->CreateTrunc(reg3, Builder->getInt64Ty());
    Value *reg4 = Builder->CreateLShr(Val, 192);
    reg4 = Builder->CreateTrunc(reg4, Builder->getInt64Ty());

    int64_t diff1 = (RegNo - AArch64::D0_D1_D2_D3) % 32;
    int64_t diff2 = (RegNo - AArch64::D0_D1_D2_D3 + 1) % 32;
    int64_t diff3 = (RegNo - AArch64::D0_D1_D2_D3 + 2) % 32;
    int64_t diff4 = (RegNo - AArch64::D0_D1_D2_D3 + 3) % 32;

    setReg(AArch64::D0 + diff1, reg1);
    setReg(AArch64::D0 + diff2, reg2);
    setReg(AArch64::D0 + diff3, reg3);
    setReg(AArch64::D0 + diff4, reg4);

    return;
  }
  DCRegisterSema::setReg(RegNo, Val);
}
