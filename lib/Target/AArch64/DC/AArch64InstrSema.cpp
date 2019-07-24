#include "AArch64InstrSema.h"
#include "AArch64RegisterSema.h"
#include "AArch64ISelLowering.h"
#include "MCTargetDesc/AArch64MCTargetDesc.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/TypeBuilder.h"
#include "AArch64GenSema.inc"
#include "MCTargetDesc/AArch64AddressingModes.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "dc-sema"

using namespace llvm;

int64_t getOnes(int64_t Num) {
    if (!Num)
        return 0;
    int64_t O = 0x1;
    while (--Num) {
        O = (O << 1) | 0x1;
    }
    return O;
}

namespace llvm {
    namespace AArch64 {
        enum NZCVShift {
            N = 31,
            Z = 30,
            C = 29,
            V = 28,
        };
    }
}

AArch64InstrSema::AArch64InstrSema(DCRegisterSema &DRS) :
        DCInstrSema(AArch64::OpcodeToSemaIdx, AArch64::InstSemantics, AArch64::ConstantArray,
                    DRS) {

}

bool AArch64InstrSema::translateTargetInst() {
    printInstruction();
    unsigned Opcode = CurrentInst->Inst.getOpcode();
    //errs() << "[+]CurrentInst->Inst: " << "\n";

    switch (Opcode) {

      //due to the size of this cases they were moved to a separate file
#include "LD_ST_cases.h"

        case AArch64::RET: {
            Builder->CreateBr(ExitBB);
            return true;
        }
        case AArch64::UBFMXri:
        case AArch64::UBFMWri:
        case AArch64::BFMWri:
        case AArch64::BFMXri:{
            //errs() << "[-----]UBFM\n";
            int64_t DstRegNo = 0;
            Value *Src = NULL;
            int64_t Rotate = NULL;
            int64_t Left = NULL;

            switch (Opcode) {
                case AArch64::UBFMXri:
                case AArch64::UBFMWri: {
                    DstRegNo = CurrentInst->Inst.getOperand(0).getReg();
                    Src = getReg(CurrentInst->Inst.getOperand(1).getReg());
                    Rotate = CurrentInst->Inst.getOperand(2).getImm();
                    Left = CurrentInst->Inst.getOperand(3).getImm();
                    break;
                }
                case AArch64::BFMXri:
                case AArch64::BFMWri: {
                    DstRegNo = CurrentInst->Inst.getOperand(0).getReg();
                    Src = getReg(CurrentInst->Inst.getOperand(2).getReg());
                    Rotate = CurrentInst->Inst.getOperand(3).getImm();
                    Left = CurrentInst->Inst.getOperand(4).getImm();
                    break;
                }
            }

            int64_t C = 0;

            Value *Result = NULL;

            if (Left >= Rotate) {
                C = getOnes(Left - Rotate + 1);
                C = C << Rotate;
                Result = Builder->CreateAnd(Src, C);
                Result = Builder->CreateLShr(Result, Rotate);
            } else {
                C = getOnes(Left + 1);
                Result = Builder->CreateAnd(Src, C);
                Result = Builder->CreateShl(Result, Src->getType()->getIntegerBitWidth() - Rotate);
                C = C << (Src->getType()->getIntegerBitWidth() - Rotate);
            }

            switch (Opcode) {
                default: {
                    break;
                }
                case AArch64::BFMWri: {
                    Value *Dst = getReg(DstRegNo);
                    C = ~C;
                    Value *Old = Builder->CreateAnd(Dst, C);
                    Result = Builder->CreateOr(Old, Result);
                    break;
                }
            }


            setReg(DstRegNo, Result);
            return true;
        }
        case AArch64::MOVKXi:
        case AArch64::MOVKWi: {
            //errs() << "[-----]MOVK\n";
            uint64_t DstRegNo = CurrentInst->Inst.getOperand(0).getReg();
            Value *Src = getReg(CurrentInst->Inst.getOperand(1).getReg());
            int64_t Imm = CurrentInst->Inst.getOperand(2).getImm();
            int64_t Shift = CurrentInst->Inst.getOperand(3).getImm();
            uint64_t Mask = ~(0xFFFFL << Shift);
            Imm = Imm << Shift;
            Value *Val = Builder->CreateAnd(Src, Mask);
            Val = Builder->CreateOr(Src, Imm);
            
            setReg(DstRegNo, Val);
            return true;
        }
        case AArch64::SBFMWri:
        case AArch64::SBFMXri: {
            //errs() << "[-----]SBFM\n";
            int64_t DstRegNo = CurrentInst->Inst.getOperand(0).getReg();
            Value *Src = getReg(CurrentInst->Inst.getOperand(1).getReg());
            int64_t Rotate = CurrentInst->Inst.getOperand(2).getImm();
            int64_t Left = CurrentInst->Inst.getOperand(3).getImm();

            uint64_t Width = Src->getType()->getIntegerBitWidth();
            if (Left >= Rotate) {
                int64_t C = getOnes(Left - Rotate + 1);
                C = C << Rotate;
                Src = Builder->CreateAnd(Src, C);
                Src = Builder->CreateLShr(Src, Rotate);
                Src = Builder->CreateTrunc(Src, Builder->getIntNTy(Left - Rotate + 1));
                Src = Builder->CreateSExt(Src, Builder->getIntNTy(Width));
            } else {
                int64_t C = getOnes(Left + 1);
                Src = Builder->CreateAnd(Src, C);
                Src = Builder->CreateTrunc(Src, Builder->getIntNTy(Left));
                Src = Builder->CreateSExt(Src, Builder->getIntNTy(Width));
                Src = Builder->CreateShl(Src, Src->getType()->getIntegerBitWidth() - Rotate);
            }
            
            setReg(DstRegNo, Src);

            return true;
        }
        case AArch64::FADDPv2i32p: {
            //errs() << "[-----]FADDPv2i32p\n";
            Value *srcReg = getReg(CurrentInst->Inst.getOperand(1).getReg());
            srcReg = Builder->CreateBitCast(srcReg, VectorType::get(Builder->getFloatTy(), 2));
            Value *res = Builder->CreateFAdd(Builder->CreateExtractElement(srcReg, (uint64_t)0), Builder->CreateExtractElement(srcReg, 1));
            
            setReg(CurrentInst->Inst.getOperand(0).getReg(), Builder->CreateBitCast(res, Builder->getInt32Ty()));
            return true;
        }
        case AArch64::FADDPv2i64p: {
            //errs() << "[-----]FADDPv2i64p\n";
            Value *srcReg = getReg(CurrentInst->Inst.getOperand(1).getReg());
            srcReg = Builder->CreateBitCast(srcReg, VectorType::get(Builder->getDoubleTy(), 2));
            Value *res = Builder->CreateFAdd(Builder->CreateExtractElement(srcReg, (uint64_t)0), Builder->CreateExtractElement(srcReg, 1));
            
            setReg(CurrentInst->Inst.getOperand(0).getReg(), Builder->CreateBitCast(res, Builder->getInt64Ty()));
            return true;
        }
            //Prefetch instructions are not handled (which is valid)
        case AArch64::PRFMl:
        case AArch64::PRFMroW:
        case AArch64::PRFMroX:
        case AArch64::PRFMui:
        case AArch64::PRFUMi: {
            return true;
        }
        case AArch64::DMB: {
            return true;
        }


        case AArch64::TBLv16i8Four:
        case AArch64::TBLv16i8One:
        case AArch64::TBLv16i8Three:
        case AArch64::TBLv16i8Two:
        case AArch64::TBLv8i8Four:
        case AArch64::TBLv8i8One:
        case AArch64::TBLv8i8Three:
        case AArch64::TBLv8i8Two:
        case AArch64::TBXv16i8Four:
        case AArch64::TBXv16i8One:
        case AArch64::TBXv16i8Three:
        case AArch64::TBXv16i8Two:
        case AArch64::TBXv8i8Four:
        case AArch64::TBXv8i8One:
        case AArch64::TBXv8i8Three:
        case AArch64::TBXv8i8Two:
        {
            //FIXME
            errs() << "TBLv16i8One not handled yet!\n";
            return true;
        }
        case AArch64::FCVTLv2i32:
        case AArch64::FCVTLv4i16:
        case AArch64::FCVTLv4i32:
        case AArch64::FCVTLv8i16:
        case AArch64::FCVTNv2i32:
        case AArch64::FCVTNv4i16:
        case AArch64::FCVTNv4i32:
        case AArch64::FCVTNv8i16: {
            //errs() << "[-----]FCVTNv\n";
            uint64_t DstRegNo = CurrentInst->Inst.getOperand(0).getReg();
            Value *Src = getReg(CurrentInst->Inst.getOperand(1).getReg());
            Value *Dst = getReg(DstRegNo);
            Src = Builder->CreateZExtOrTrunc(Src, Dst->getType());
            setReg(DstRegNo, Src);
            return true;
        }
        case AArch64::FMLAv4i32_indexed:
        case AArch64::FMLSv1i32_indexed:
        case AArch64::FMLSv1i64_indexed:
        case AArch64::FMLAv2i32_indexed:
        case AArch64::FMLAv2i64_indexed:
        case AArch64::FMLAv1i32_indexed:
        case AArch64::FMLAv1i64_indexed:
        {
            return true;
        }
        case AArch64::SHLLv16i8:
        case AArch64::SHLLv2i32:
        case AArch64::SHLLv4i16:
        case AArch64::SHLLv4i32:
        case AArch64::SHLLv8i16:
        case AArch64::SHLLv8i8:
        {
            return true;
        }
        case AArch64::SHRNv8i16_shift:
        {
            return true;
        }
        case AArch64::SQXTUNv16i8:
        case AArch64::SQXTNv8i16:
        case AArch64::UQXTNv8i16:
        {

            return true;
        }
    }
    if (Idx) {
        DEBUG(errs() << "Use table\n");
        return false;
    }
    // llvm_unreachable("not handled yet");
    //FIXME: unhandled instruction should lead to an error...
    return true;
}

void AArch64InstrSema::translateCustomOperand(unsigned OperandType, unsigned MIOperandNo) {
    switch (OperandType) {
        default: {
            errs() << "Unhandled OpType: " << OperandType << "\n";
            errs() << utohexstr(CurrentInst->Address) << "\n";
            llvm_unreachable("");
            break;
        }
        case AArch64::OpTypes::SUBanonymous_745:
        //arith_extended_reg32
        case AArch64::OpTypes::ADDanonymous_745:
        //arith_extended_reg32to64<i64>
        case AArch64::OpTypes::ADDanonymous_746: {

            Value *Reg = getReg(getRegOp(MIOperandNo));
            uint64_t arith_extend64 = AArch64_AM::getArithExtendType(getImmOp(MIOperandNo + 1));
            Type *ExtType = NULL;

            switch (OperandType) {
                case AArch64::OpTypes::ADDanonymous_746: {
                    DEBUG(errs() << "Operand:ADDanonymous_746\n");
                    ExtType = Builder->getInt64Ty();
                    break;
                }
                case AArch64::OpTypes::SUBanonymous_745: {
                    DEBUG(errs() << "Operand:ADDanonymous_745\n");
                    ExtType = Builder->getInt32Ty();
                    break;
                }
                case AArch64::OpTypes::ADDanonymous_745: {
                    DEBUG(errs() << "Operand:ADDanonymous_745\n");
                    ExtType = Builder->getInt32Ty();
                    break;
                }
            }

            registerResult(ArithExtend(Reg, ExtType, arith_extend64));
            break;
        }
        //arith_extended_reg32to64
        case AArch64::OpTypes::SUBanonymous_746:
        //arith_extended_reg32
        case AArch64::OpTypes::SUBSanonymous_755:
        case AArch64::OpTypes::SUBSanonymous_756:
        case AArch64::OpTypes::ADDSanonymous_755:
        case AArch64::OpTypes::ADDSanonymous_756:{
            DEBUG(errs() << "Operand:SUBSanonymous_755\n");
            Value *Reg = getReg(getRegOp(MIOperandNo));
            uint64_t arith_extend64 = AArch64_AM::getArithExtendType(getImmOp(MIOperandNo + 1));

            Type *ExtType = NULL;

            switch (OperandType) {
                case AArch64::OpTypes::SUBSanonymous_755: {
                    DEBUG(errs() << "Operand:SUBSanonymous_755\n");
                    ExtType = Builder->getInt32Ty();
                    break;
                }
                case AArch64::OpTypes::SUBanonymous_746: {
                    DEBUG(errs() << "Operand:SUBanonymous_746\n");
                    ExtType = Builder->getInt64Ty();
                    break;
                }
                case AArch64::OpTypes::SUBSanonymous_756: {
                    DEBUG(errs() << "Operand:SUBSanonymous_756\n");
                    ExtType = Builder->getInt64Ty();
                    break;
                }
                //BugID: vlc_0x100DFAB6C
                case AArch64::OpTypes::ADDSanonymous_755: {
                    DEBUG(errs() << "Operand:ADDSanonymous_755\n");
                    ExtType = Builder->getInt32Ty();
                    break;
                }
                case AArch64::OpTypes::ADDSanonymous_756: {
                    DEBUG(errs() << "Operand:ADDSanonymous_756\n");
                    ExtType = Builder->getInt64Ty();
                    break;
                }
            }
            registerResult(ArithExtend(Reg, ExtType, arith_extend64));

            break;
        }
        case AArch64::OpTypes::VectorIndex1: {
            DEBUG(errs() << "Operand:VectorIndex1\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::VectorIndexB: {
            DEBUG(errs() << "Operand:VectorIndexB\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::VectorIndexD: {
            DEBUG(errs() << "Operand:VectorIndexD\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::VectorIndexH: {
            DEBUG(errs() << "Operand:VectorIndexH\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::VectorIndexS: {
            DEBUG(errs() << "Operand:VectorIndexS\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::addsub_shifted_imm32:
        case AArch64::OpTypes::addsub_shifted_imm64:{
            switch (OperandType) {
                case AArch64::OpTypes::addsub_shifted_imm32:
                    DEBUG(errs() << "Operand:addsub_shifted_imm32\n");
                    break;
                case AArch64::OpTypes::addsub_shifted_imm64:
                    DEBUG(errs() << "Operand:addsub_shifted_imm64\n");
                    break;
            }

            uint64_t Imm = getImmOp(MIOperandNo);
            uint64_t Shift = getImmOp(MIOperandNo + 1);

            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst =
                    ConstantInt::get(cast<IntegerType>(ResType), Imm << Shift);
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::addsub_shifted_imm32_neg: {
            DEBUG(errs() << "Operand:addsub_shifted_imm32_neg\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::addsub_shifted_imm64_neg: {
            DEBUG(errs() << "Operand:addsub_shifted_imm64_neg\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::adrlabel: {
            DEBUG(errs() << "Operand:adrlabel\n");
            uint64_t Target = CurrentInst->Address + (getImmOp(MIOperandNo));
            registerResult(Builder->getInt64(Target));
            break;
        }
        case AArch64::OpTypes::adrplabel: {
            DEBUG(errs() << "Operand:adrplabel\n");
            uint64_t Offset = getImmOp(MIOperandNo) << 12;
            registerResult(Builder->getInt64(Offset));
            break;
        }
        case AArch64::OpTypes::am_bl_target: {
            DEBUG(errs() << "Operand:am_bl_target\n");
            uint64_t Target = CurrentInst->Address + (getImmOp(MIOperandNo) * 4);
            registerResult(Builder->getInt64(Target));
            break;
        }
        case AArch64::OpTypes::am_b_target: {
            DEBUG(errs() << "Operand:am_b_target\n");
            uint64_t Target = CurrentInst->Address + (getImmOp(MIOperandNo) * 4);
            registerResult(Builder->getInt64(Target));
            break;
        }
        case AArch64::OpTypes::am_brcond: {
            DEBUG(errs() << "Operand:am_brcond\n");
            uint64_t Target = CurrentInst->Address + (getImmOp(MIOperandNo) * 4);
            registerResult(getOrCreateBasicBlock(Target));
            registerResult(getOrCreateBasicBlock(getBasicBlockEndAddress()));
            break;
        }
        case AArch64::OpTypes::am_ldrlit: {
            DEBUG(errs() << "Operand:am_ldrlit\n");
            uint64_t Target = CurrentInst->Address + (getImmOp(MIOperandNo) * 4);
            registerResult(Builder->getInt64(Target));
            break;
        }
        case AArch64::OpTypes::am_tbrcond: {
            DEBUG(errs() << "Operand:am_tbrcond\n");
            uint64_t Target = CurrentInst->Address + (getImmOp(MIOperandNo) * 4);
            registerResult(Builder->getInt64(Target));
            break;
        }
        case AArch64::OpTypes::anonymous_1014_movimm: {
            DEBUG(errs() << "Operand:anonymous_1014_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1015_movimm: {
            DEBUG(errs() << "Operand:anonymous_1015_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1016_movimm: {
            DEBUG(errs() << "Operand:anonymous_1016_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1017_movimm: {
            DEBUG(errs() << "Operand:anonymous_1017_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1018_movimm: {
            DEBUG(errs() << "Operand:anonymous_1018_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1019_movimm: {
            DEBUG(errs() << "Operand:anonymous_1019_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1020_movimm: {
            DEBUG(errs() << "Operand:anonymous_1020_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1021_movimm: {
            DEBUG(errs() << "Operand:anonymous_1021_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1022_movimm: {
            DEBUG(errs() << "Operand:anonymous_1022_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1023_movimm: {
            DEBUG(errs() << "Operand:anonymous_1023_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1024_movimm: {
            DEBUG(errs() << "Operand:anonymous_1024_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::anonymous_1025_movimm: {
            DEBUG(errs() << "Operand:anonymous_1025_movimm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::arith_extend: {
            DEBUG(errs() << "Operand:arith_extend\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::arith_extend64: {
            DEBUG(errs() << "Operand:arith_extend64\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::arith_extendlsl64: {
            DEBUG(errs() << "Operand:arith_extendlsl64\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::arith_shift32: {
            DEBUG(errs() << "Operand:arith_shift32\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::arith_shift64: {
            DEBUG(errs() << "Operand:arith_shift64\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::arith_shifted_reg32:
        case AArch64::OpTypes::arith_shifted_reg64: {
            switch (OperandType) {
                case AArch64::OpTypes::arith_shifted_reg64:
                    DEBUG(errs() << "Operand:arith_shifted_reg64\n");
                    break;
                case AArch64::OpTypes::arith_shifted_reg32:
                    DEBUG(errs() << "Operand:arith_shifted_reg32\n");
                    break;
            }
            Value *OpReg = getReg(getRegOp(MIOperandNo));
            OpReg = Builder->CreateShl(OpReg, getImmOp(MIOperandNo + 1));
            registerResult(OpReg);
            break;
        }
        case AArch64::OpTypes::barrier_op: {
            DEBUG(errs() << "Operand:barrier_op\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::ccode: {
            DEBUG(errs() << "Operand:ccode\n");
            uint64_t CC = getImmOp(MIOperandNo);
            Value *Cmp = NULL;
            switch (CC) {
                case AArch64CC::EQ: {
                    DEBUG(errs() << "CC: EQ\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *Z_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::Z));
                    Cmp = Builder->CreateICmpNE(Z_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::NE: {
                    DEBUG(errs() << "CC: NE\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *Z_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::Z));
                    Cmp = Builder->CreateICmpEQ(Z_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::HS: {
                    DEBUG(errs() << "CC: HS\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *C_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::C));
                    Cmp = Builder->CreateICmpNE(C_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::LO: {
                    DEBUG(errs() << "CC: LO\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *C_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::C));
                    Cmp = Builder->CreateICmpEQ(C_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::MI: {
                    DEBUG(errs() << "CC: MI\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *N_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::N));
                    Cmp = Builder->CreateICmpNE(N_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::PL: {
                    DEBUG(errs() << "CC: PL\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *N_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::N));
                    Cmp = Builder->CreateICmpEQ(N_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::VS: {
                    DEBUG(errs() << "CC: VS\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *V_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::V));
                    Cmp = Builder->CreateICmpNE(V_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::VC: {
                    DEBUG(errs() << "CC: VC\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *V_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::V));
                    Cmp = Builder->CreateICmpEQ(V_flag, Builder->getInt32(0));
                    break;
                }
                case AArch64CC::HI: {
                    DEBUG(errs() << "CC: HI\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *C_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::C));
                    Value *Z_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::Z));
                    Cmp = Builder->CreateAnd(Builder->CreateICmpNE(C_flag, Builder->getInt32(0)), Builder->CreateICmpEQ(Z_flag, Builder->getInt32(0)));
                    break;
                }
                case AArch64CC::LS: {
                    DEBUG(errs() << "CC: LS\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *C_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::C));
                    Value *Z_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::Z));
                    Cmp = Builder->CreateOr(Builder->CreateICmpEQ(C_flag, Builder->getInt32(0)), Builder->CreateICmpNE(Z_flag, Builder->getInt32(0)));
                    break;
                }
                case AArch64CC::GE: {
                    DEBUG(errs() << "CC: GE\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *N_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::N));
                    Value *V_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::V));
                    Cmp = Builder->CreateICmpEQ(N_flag, V_flag);
                    break;
                }
                case AArch64CC::LT: {
                    DEBUG(errs() << "CC: LT\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *N_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::N));
                    Value *V_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::V));
                    Cmp = Builder->CreateICmpNE(N_flag, V_flag);
                    break;
                }
                case AArch64CC::GT: {
                    DEBUG(errs() << "CC: GT\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *Z_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::Z));
                    Value *N_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::N));
                    Value *V_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::V));
                    Cmp = Builder->CreateICmpEQ(N_flag, V_flag);
                    Cmp = Builder->CreateAnd(Builder->CreateICmpEQ(Z_flag, Builder->getInt32(0)), Cmp);
                    break;
                }
                case AArch64CC::LE: {
                    DEBUG(errs() << "CC: LE\n");
                    Value *NZCV = getReg(AArch64::NZCV);
                    Value *Z_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::Z));
                    Value *N_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::N));
                    Value *V_flag = Builder->CreateAnd(NZCV, Builder->getInt32(0x1 << AArch64::NZCVShift::V));
                    Cmp = Builder->CreateOr(Builder->CreateICmpNE(Z_flag, Builder->getInt32(0)), Builder->CreateICmpNE(N_flag, V_flag));
                    break;
                }
                case AArch64CC::AL: {
                    DEBUG(errs() << "CC: AL\n");
                    Cmp = Builder->getInt1(true);
                    break;
                }
                case AArch64CC::NV: {
                    DEBUG(errs() << "CC: NV\n");
                    llvm_unreachable("");
                    break;
                }
            }
            assert(Cmp);
            registerResult(Cmp);
            break;
        }
        case AArch64::OpTypes::f32imm: {
            DEBUG(errs() << "Operand:f32imm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::f64imm: {
            DEBUG(errs() << "Operand:f64imm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::fixedpoint_f32_i32: {
            DEBUG(errs() << "Operand:fixedpoint_f32_i32\n");
            uint64_t FBits = getImmOp(MIOperandNo);
            registerResult(ConstantFP::get(Builder->getFloatTy(), pow(2.0, FBits)));
            break;
        }
        case AArch64::OpTypes::fixedpoint_f32_i64: {
            DEBUG(errs() << "Operand:fixedpoint_f32_i64\n");
            uint64_t FBits = getImmOp(MIOperandNo);
            registerResult(ConstantFP::get(Builder->getFloatTy(), pow(2.0, FBits)));
            break;
        }
        case AArch64::OpTypes::fixedpoint_f64_i32: {
            DEBUG(errs() << "Operand:fixedpoint_f64_i32\n");
            uint64_t FBits = getImmOp(MIOperandNo);
            registerResult(ConstantFP::get(Builder->getDoubleTy(), pow(2.0, FBits)));
            break;
        }
        case AArch64::OpTypes::fixedpoint_f64_i64: {
            DEBUG(errs() << "Operand:fixedpoint_f64_i64\n");
            uint64_t FBits = getImmOp(MIOperandNo);
            registerResult(ConstantFP::get(Builder->getDoubleTy(), pow(2.0, FBits)));
            break;
        }
        case AArch64::OpTypes::fpimm32:
        case AArch64::OpTypes::fpimm64: {

            float FPImm = AArch64_AM::getFPImmFloat(getImmOp(MIOperandNo));
            Type *Ty = NULL;
            switch (OperandType) {
                case AArch64::OpTypes::fpimm64: {
                    DEBUG(errs() << "Operand:fpimm64\n");
                    Ty = Builder->getDoubleTy();
                    break;
                }
                case AArch64::OpTypes::fpimm32: {
                    DEBUG(errs() << "Operand:fpimm32\n");
                    Ty = Builder->getFloatTy();
                    break;
                }
            }
            registerResult(ConstantFP::get(Ty, FPImm));
            break;
        }
        case AArch64::OpTypes::fpimm8: {
            DEBUG(errs() << "Operand:fpimm8\n");
            registerResult(Builder->getInt8(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::i16imm: {
            DEBUG(errs() << "Operand:i16imm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i1imm: {
            DEBUG(errs() << "Operand:i1imm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i32imm: {
            DEBUG(errs() << "Operand:i32imm\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::i32shift_a: {
            DEBUG(errs() << "Operand:i32shift_a\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i32shift_b: {
            DEBUG(errs() << "Operand:i32shift_b\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i32shift_sext_i16: {
            DEBUG(errs() << "Operand:i32shift_sext_i16\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i32shift_sext_i8: {
            DEBUG(errs() << "Operand:i32shift_sext_i8\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i64imm: {
            DEBUG(errs() << "Operand:i64imm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i64shift_a: {
            DEBUG(errs() << "Operand:i64shift_a\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i64shift_b: {
            DEBUG(errs() << "Operand:i64shift_b\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i64shift_sext_i16: {
            DEBUG(errs() << "Operand:i64shift_sext_i16\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i64shift_sext_i32: {
            DEBUG(errs() << "Operand:i64shift_sext_i32\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i64shift_sext_i8: {
            DEBUG(errs() << "Operand:i64shift_sext_i8\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::i8imm: {
            DEBUG(errs() << "Operand:i8imm\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::imm0_127: {
            DEBUG(errs() << "Operand:imm0_127\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::imm0_15: {
            DEBUG(errs() << "Operand:imm0_15\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::imm0_255: {
            DEBUG(errs() << "Operand:imm0_255\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::imm0_31: {
            DEBUG(errs() << "Operand:imm0_31\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::imm0_63: {
            DEBUG(errs() << "Operand:imm0_63\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::imm0_65535: {
            DEBUG(errs() << "Operand:imm0_65535\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::imm0_7: {
            DEBUG(errs() << "Operand:imm0_7\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::imm32_0_15:
        case AArch64::OpTypes::imm32_0_31: {
            switch (OperandType) {
                case AArch64::OpTypes::imm32_0_15:
                    DEBUG(errs() << "Operand:imm32_0_15\n");
                    break;
                case AArch64::OpTypes::imm32_0_31:
                    DEBUG(errs() << "Operand:imm32_0_31\n");
                    break;
            }
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::inv_ccode: {
            DEBUG(errs() << "Operand:inv_ccode\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::logical_imm32: {
            DEBUG(errs() << "Operand:logical_imm32\n");
            uint64_t logical_imm = AArch64_AM::decodeLogicalImmediate(getImmOp(MIOperandNo), 32);
            registerResult(Builder->getInt32(logical_imm));
            break;
        }
        case AArch64::OpTypes::logical_imm32_not: {
            DEBUG(errs() << "Operand:logical_imm32_not\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::logical_imm64: {
            DEBUG(errs() << "Operand:logical_imm64\n");
            uint64_t logical_imm = AArch64_AM::decodeLogicalImmediate(getImmOp(MIOperandNo), 64);
            registerResult(Builder->getInt64(logical_imm));
            break;
        }
        case AArch64::OpTypes::logical_imm64_not: {
            DEBUG(errs() << "Operand:logical_imm64_not\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::logical_shift32: {
            DEBUG(errs() << "Operand:logical_shift32\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::logical_shift64: {
            DEBUG(errs() << "Operand:logical_shift64\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::logical_shifted_reg32:
        case AArch64::OpTypes::logical_shifted_reg64: {
            switch (OperandType) {
                case AArch64::OpTypes::logical_shifted_reg64:
                    DEBUG(errs() << "Operand:logical_shifted_reg64\n");
                    break;
                case AArch64::OpTypes::logical_shifted_reg32:
                    DEBUG(errs() << "Operand:logical_shifted_reg32\n");
                    break;
            }

            Value *Reg = getReg(getRegOp(MIOperandNo));
            uint64_t Shift = getImmOp(MIOperandNo + 1);

            if (Shift > Reg->getType()->getIntegerBitWidth() * 3) {
                Shift = Shift % Reg->getType()->getIntegerBitWidth();
                Value *LSBits = Builder->getInt(APInt(Reg->getType()->getIntegerBitWidth(), getOnes(Shift)));
                LSBits = Builder->CreateAnd(Reg, LSBits);
                LSBits = Builder->CreateShl(LSBits, Reg->getType()->getIntegerBitWidth() - Shift);
                Reg = Builder->CreateLShr(Reg, Shift);
                Reg = Builder->CreateOr(Reg, LSBits);
            } else if (Shift > Reg->getType()->getIntegerBitWidth() * 2) {
                Reg = Builder->CreateAShr(Reg, Shift % Reg->getType()->getIntegerBitWidth());
            } else if (Shift > Reg->getType()->getIntegerBitWidth()) {
                Reg = Builder->CreateLShr(Reg, Shift % Reg->getType()->getIntegerBitWidth());
            } else {
                Reg = Builder->CreateShl(Reg, Shift);
            }
            registerResult(Reg);
            break;
        }
        case AArch64::OpTypes::logical_vec_hw_shift: {
            DEBUG(errs() << "Operand:logical_vec_hw_shift\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::logical_vec_shift: {
            DEBUG(errs() << "Operand:logical_vec_shift\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::maski16_or_more: {
            DEBUG(errs() << "Operand:maski16_or_more\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::maski8_or_more: {
            DEBUG(errs() << "Operand:maski8_or_more\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::move_vec_shift: {
            DEBUG(errs() << "Operand:move_vec_shift\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst =
                    ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo)- 256);
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::movimm32_imm: {
            DEBUG(errs() << "Operand:movimm32_imm\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst =
                    ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::movimm32_shift:
        case AArch64::OpTypes::movimm64_shift: {
            switch (OperandType) {
                case AArch64::OpTypes::movimm32_shift: {
                    DEBUG(errs() << "Operand:movimm32_shift\n");
                    break;
                }
                case AArch64::OpTypes::movimm64_shift: {
                    DEBUG(errs() << "Operand:movimm64_shift\n");
                    break;
                }
            }

            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst =
                    ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::movk_symbol_g0: {
            DEBUG(errs() << "Operand:movk_symbol_g0\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movk_symbol_g1: {
            DEBUG(errs() << "Operand:movk_symbol_g1\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movk_symbol_g2: {
            DEBUG(errs() << "Operand:movk_symbol_g2\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movk_symbol_g3: {
            DEBUG(errs() << "Operand:movk_symbol_g3\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movz_symbol_g0: {
            DEBUG(errs() << "Operand:movz_symbol_g0\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movz_symbol_g1: {
            DEBUG(errs() << "Operand:movz_symbol_g1\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movz_symbol_g2: {
            DEBUG(errs() << "Operand:movz_symbol_g2\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::movz_symbol_g3: {
            DEBUG(errs() << "Operand:movz_symbol_g3\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::mrs_sysreg_op: {
            DEBUG(errs() << "Operand:mrs_sysreg_op\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::msr_sysreg_op: {
            DEBUG(errs() << "Operand:msr_sysreg_op\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::neg_addsub_shifted_imm32: {
            DEBUG(errs() << "Operand:neg_addsub_shifted_imm32\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::neg_addsub_shifted_imm64: {
            DEBUG(errs() << "Operand:neg_addsub_shifted_imm64\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::prfop: {
            DEBUG(errs() << "Operand:prfop\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::pstatefield_op: {
            DEBUG(errs() << "Operand:pstatefield_op\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::ro_Wextend128:
        case AArch64::OpTypes::ro_Xextend128:
        case AArch64::OpTypes::ro_Wextend16:
        case AArch64::OpTypes::ro_Wextend64:
        case AArch64::OpTypes::ro_Wextend8:
        case AArch64::OpTypes::ro_Xextend16:
        case AArch64::OpTypes::ro_Xextend32:
        case AArch64::OpTypes::ro_Xextend64:
        case AArch64::OpTypes::ro_Xextend8:
        case AArch64::OpTypes::ro_Wextend32: {
            uint64_t ShiftScale = 0;
            uint64_t Ext = -1U;
            switch (OperandType) {
                case AArch64::OpTypes::ro_Wextend8:
                    DEBUG(errs() << "Operand:ro_Wextend8\n");
                    ShiftScale = 0;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTW : AArch64_AM::UXTW;
                    break;
                case AArch64::OpTypes::ro_Wextend16:
                    DEBUG(errs() << "Operand:ro_Wextend16\n");
                    ShiftScale = 1;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTW : AArch64_AM::UXTW;
                    break;
                case AArch64::OpTypes::ro_Wextend32:
                    DEBUG(errs() << "Operand:ro_Wextend32\n");
                    ShiftScale = 2;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTW : AArch64_AM::UXTW;
                    break;
                case AArch64::OpTypes::ro_Wextend64:
                    DEBUG(errs() << "Operand:ro_Wextend64\n");
                    ShiftScale = 3;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTW : AArch64_AM::UXTW;
                    break;
                case AArch64::OpTypes::ro_Wextend128:
                    DEBUG(errs() << "Operand:ro_Wextend128\n");
                    ShiftScale = 4;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTW : AArch64_AM::UXTW;
                    break;
                case AArch64::OpTypes::ro_Xextend8:
                    DEBUG(errs() << "Operand:ro_Xextend8\n");
                    ShiftScale = 0;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTX : AArch64_AM::UXTX;
                    break;
                case AArch64::OpTypes::ro_Xextend16:
                    DEBUG(errs() << "Operand:ro_Xextend16\n");
                    ShiftScale = 1;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTX : AArch64_AM::UXTX;
                    break;
                case AArch64::OpTypes::ro_Xextend32:
                    DEBUG(errs() << "Operand:ro_Xextend32\n");
                    ShiftScale = 2;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTX : AArch64_AM::UXTX;
                    break;
                case AArch64::OpTypes::ro_Xextend64:
                    DEBUG(errs() << "Operand:ro_Xextend64\n");
                    ShiftScale = 3;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTX : AArch64_AM::UXTX;
                    break;
                case AArch64::OpTypes::ro_Xextend128:
                    DEBUG(errs() << "Operand:ro_Xextend128\n");
                    ShiftScale = 4;
                    Ext =  getImmOp(MIOperandNo) ? AArch64_AM::SXTX : AArch64_AM::UXTX;
                    break;
            }
            assert(ShiftScale || OperandType == AArch64::OpTypes::ro_Xextend8 || OperandType == AArch64::OpTypes::ro_Wextend8);
            assert(Ext != -1U);
            uint64_t Shift = getImmOp(MIOperandNo + 1) * ShiftScale;
            registerResult(Builder->getInt64((Ext << 32) | Shift));
            break;
        }
        case AArch64::OpTypes::simdimmtype10: {
            DEBUG(errs() << "Operand:simdimmtype10\n");
            uint64_t Imm = getImmOp(MIOperandNo);
            uint64_t V = 0;
            for (unsigned i = 0; i < 8; ++i) {
                if (Imm & (0x1 << i)) {
                    V |= (0xFF << (i * 8));
                }
            }
            registerResult(Builder->getInt64(V));
            break;
        }
        case AArch64::OpTypes::simm7s16: {
            DEBUG(errs() << "Operand:simm7s16\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo) * 16));
            break;
        }
        case AArch64::OpTypes::simm9: {
            DEBUG(errs() << "Operand:simm9\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst =
                    ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::simm9_offset_fb128: {
            DEBUG(errs() << "Operand:simm9_offset_fb128\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::simm9_offset_fb16: {
            DEBUG(errs() << "Operand:simm9_offset_fb16\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::simm9_offset_fb32: {
            DEBUG(errs() << "Operand:simm9_offset_fb32\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::simm9_offset_fb64: {
            DEBUG(errs() << "Operand:simm9_offset_fb64\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::simm9_offset_fb8: {
            DEBUG(errs() << "Operand:simm9_offset_fb8\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::sys_cr_op: {
            DEBUG(errs() << "Operand:sys_cr_op\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::tbz_imm0_31_diag: {
            DEBUG(errs() << "Operand:tbz_imm0_31_diag\n");
            uint64_t BitNo = getImmOp(MIOperandNo);
            uint64_t Mask = 0x1L << BitNo;
            registerResult(Builder->getInt32(Mask));
            break;
        }
        case AArch64::OpTypes::tbz_imm0_31_nodiag: {
            DEBUG(errs() << "Operand:tbz_imm0_31_nodiag\n");
            llvm_unreachable("Operand not implemented");
            break;
        }
        case AArch64::OpTypes::tbz_imm32_63: {
            DEBUG(errs() << "Operand:tbz_imm32_63\n");
            uint64_t BitNo = getImmOp(MIOperandNo);
            uint64_t Mask = 0x1L << BitNo;
            registerResult(Builder->getInt64(Mask));
            break;
            break;
        }
        case AArch64::OpTypes::uimm12s1:
        case AArch64::OpTypes::uimm12s16:
        case AArch64::OpTypes::uimm12s2:
        case AArch64::OpTypes::uimm12s4:
        case AArch64::OpTypes::uimm12s8:
        case AArch64::OpTypes::simm7s4:
        case AArch64::OpTypes::simm7s8:{

            uint64_t Scale = 0;

            switch (OperandType) {
                case AArch64::OpTypes::uimm12s1:
                    DEBUG(errs() << "Operand:uimm12s1\n");
                    Scale = 1;
                    break;
                case AArch64::OpTypes::uimm12s16:
                    DEBUG(errs() << "Operand:uimm12s16\n");
                    Scale = 16;
                    break;
                case AArch64::OpTypes::uimm12s2:
                    DEBUG(errs() << "Operand:uimm12s2\n");
                    Scale = 2;
                    break;
                case AArch64::OpTypes::uimm12s8:
                    DEBUG(errs() << "Operand:uimm12s8\n");
                    Scale = 8;
                    break;
                case AArch64::OpTypes::uimm12s4:
                    DEBUG(errs() << "Operand:uimm12s4\n");
                    Scale = 4;
                    break;
                case AArch64::OpTypes::simm7s4:
                    DEBUG(errs() << "Operand:simm7s4\n");
                    Scale = 4;
                    break;
                case AArch64::OpTypes::simm7s8:
                    DEBUG(errs() << "Operand:simm7s8\n");
                    Scale = 8;
                    break;
            }
            assert(Scale);
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst =
                    ConstantInt::get(cast<IntegerType>(ResType), Scale * getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::vecshiftL16: {
            DEBUG(errs() << "Operand:vecshiftL16\n");
            registerResult(Builder->getInt32(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftL32: {
            DEBUG(errs() << "Operand:vecshiftL32\n");
            registerResult(Builder->getInt32(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftL64: {
            DEBUG(errs() << "Operand:vecshiftL64\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftL8: {
            DEBUG(errs() << "Operand:vecshiftL8\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftR16: {
            DEBUG(errs() << "Operand:vecshiftR16\n");
            registerResult(Builder->getInt32(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftR16Narrow: {
            DEBUG(errs() << "Operand:vecshiftR16Narrow\n");
            registerResult(Builder->getInt32(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftR32: {
            DEBUG(errs() << "Operand:vecshiftR32\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::vecshiftR32Narrow: {
            DEBUG(errs() << "Operand:vecshiftR32Narrow\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::vecshiftR64: {
            DEBUG(errs() << "Operand:vecshiftR64\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
        case AArch64::OpTypes::vecshiftR64Narrow: {
            DEBUG(errs() << "Operand:vecshiftR64Narrow\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Cst = ConstantInt::get(cast<IntegerType>(ResType), getImmOp(MIOperandNo));
            registerResult(Cst);
            break;
        }
        case AArch64::OpTypes::vecshiftR8: {
            DEBUG(errs() << "Operand:vecshiftR8\n");
            registerResult(Builder->getInt64(getImmOp(MIOperandNo)));
            break;
        }
    }
}

Value *AArch64InstrSema::getNZCVFlags(Value *Result, Value *LHS, Value *RHS) {
    Type *ResType = Result->getType();
    Value *Zero =
            ConstantInt::get(cast<IntegerType>(ResType), 0);
    Value *Z_flag = Builder->CreateICmpEQ(Result, Zero);
    Value *N_flag = Builder->CreateICmpSLT(Result, Zero);

    Value *C_flag = NULL;
    Value *V_flag = NULL;
    if (LHS && RHS) {
        std::vector<Value*> args;
        args.push_back(LHS);
        args.push_back(RHS);
        std::vector<Type*> types;
        types.push_back(LHS->getType());
        Value *usub = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, Intrinsic::usub_with_overflow, types), args);
        usub = Builder->CreateExtractValue(usub, 1, "unsigned_overflow");
        usub = Builder->CreateNot(usub);

        Value *ssub = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, Intrinsic::ssub_with_overflow, types), args);
        ssub = Builder->CreateExtractValue(ssub, 1, "signed_overflow");
        C_flag = usub;
        V_flag = ssub;
    } else {
        C_flag = Builder->getInt1(false);
        V_flag = Builder->getInt1(false);
    }

    return getNZCVFlag(N_flag, Z_flag, C_flag, V_flag);
}


Value *AArch64InstrSema::getNZCVFlag(Value *N, Value *Z, Value *C, Value *V) {
    if (C == NULL) {
        C = Builder->getInt1(false);
    }
    if (V == NULL) {
        V = Builder->getInt1(false);
    }

    Value *NZCV = Builder->CreateSelect(N, Builder->getInt32(0x1 << AArch64::NZCVShift::N), Builder->getInt32(0));
    NZCV = Builder->CreateOr(NZCV, Builder->CreateSelect(Z, Builder->getInt32(0x1 << AArch64::NZCVShift::Z), Builder->getInt32(0)));
    NZCV = Builder->CreateOr(NZCV, Builder->CreateSelect(C, Builder->getInt32(0x1 << AArch64::NZCVShift::C), Builder->getInt32(0)));
    NZCV = Builder->CreateOr(NZCV, Builder->CreateSelect(V, Builder->getInt32(0x1 << AArch64::NZCVShift::V), Builder->getInt32(0)));

    return NZCV;
}

Value *AArch64InstrSema::ArithExtend(Value *Value, Type *ExtType, uint64_t Ext) {
    switch (Ext) {
        default:
            errs() << Ext << "\n";
            llvm_unreachable("");
        case AArch64_AM::UXTW: {
            Value = Builder->CreateZExt(Value, ExtType);
            break;
        }
        case AArch64_AM::SXTW: {
            Value = Builder->CreateSExt(Value, ExtType);
            break;
        }
        case AArch64_AM::SXTB: {
            Value = Builder->CreateTrunc(Value, Builder->getInt8Ty());
            Value = Builder->CreateSExt(Value, ExtType);
            break;
        }
        case AArch64_AM::UXTB: {
            Value = Builder->CreateTrunc(Value, Builder->getInt8Ty());
            Value = Builder->CreateZExt(Value, ExtType);
        }
        case AArch64_AM::SXTH: {
            Value = Builder->CreateTrunc(Value, Builder->getInt16Ty());
            Value = Builder->CreateSExt(Value, ExtType);
            break;
        }
        case AArch64_AM::UXTH: {
            Value = Builder->CreateTrunc(Value, Builder->getInt16Ty());
            Value = Builder->CreateZExt(Value, ExtType);
            break;
        }
    }
    return Value;
}

Value *AArch64InstrSema::FPCompare(Value * LHS, Value * RHS) {
    Value *Sub = Builder->CreateFSub(LHS, RHS);

    Value *Zero = ConstantFP::get(LHS->getType(), 0.0);

    Value *Z_flag = Builder->CreateFCmpOEQ(Sub, Zero);
    Value *N_flag = Builder->CreateFCmpOLT(Sub, Zero);
    //TODO: C and V flag!

    return getNZCVFlag(N_flag, Z_flag);
}

// 是否在这里进行了特殊处理
void AArch64InstrSema::translateTargetOpcode() {
    //errs()<<"AArch64 special code translate \n";
    switch (Opcode) {
        default:
            errs() << Opcode << "\n";
            llvm_unreachable("unknown opcode!");
        case AArch64ISD::ADDS: {
            ResEVT = NextVT();
            Value *V1 = getNextOperand();
            Value *V2 = getNextOperand();
            Value *Result = Builder->CreateBinOp(Instruction::Sub, V1, V2);
            registerResult(Result);
            registerResult(getNZCVFlags(Result, V1, V2));
            break;
        }
        case AArch64ISD::CALL: {
            insertCall(getNextOperand());
            break;
        }
        case AArch64ISD::SUBS: {
            ResEVT = NextVT();
            Value *V1 = getNextOperand();
            Value *V2 = getNextOperand();
            Value *Result = Builder->CreateBinOp(Instruction::Sub, V1, V2);
            registerResult(Result);
            registerResult(getNZCVFlags(Result, V1, V2));
            break;
        }
        case AArch64ISD::BRCOND: {
            BasicBlock *TrueBBlock = dyn_cast<BasicBlock>(getNextOperand());
            BasicBlock *FalseBBlock = dyn_cast<BasicBlock>(getNextOperand());
            Value *Cond = getNextOperand();
            Builder->CreateCondBr(Cond, TrueBBlock, FalseBBlock);
            break;
        }
        case AArch64ISD::CBNZ:
        case AArch64ISD::CBZ: {
            Value *Reg = getNextOperand();
            BasicBlock *TrueBBlock = dyn_cast<BasicBlock>(getNextOperand());
            Value *Zero = ConstantInt::get(Reg->getType(), 0);
            switch (Opcode) {
                case AArch64ISD::CBNZ: {
                    Builder->CreateCondBr(Builder->CreateICmpNE(Reg, Zero), TrueBBlock, getOrCreateBasicBlock(getBasicBlockEndAddress()));
                    break;
                }
                case AArch64ISD::CBZ: {
                    Builder->CreateCondBr(Builder->CreateICmpEQ(Reg, Zero), TrueBBlock, getOrCreateBasicBlock(getBasicBlockEndAddress()));
                    break;
                }
            }
            break;
        }
        case AArch64ISD::CCMP: {
            Value *LHS = getNextOperand();
            Value *RHS = getNextOperand();
            Value *NZCVfalse = getNextOperand();
            NZCVfalse = Builder->CreateShl(NZCVfalse, 28);
            Value *Cond = getNextOperand();
            //TODO: not used?
            Value *NZCVold = getNextOperand();

            Value *Result = Builder->CreateSub(LHS, RHS);
            Value *NZCVtrue = getNZCVFlags(Result, LHS, RHS);

            registerResult(Builder->CreateSelect(Cond, NZCVtrue, NZCVfalse));

            break;
        }
        case AArch64ISD::WrapperLarge: {
            DEBUG(errs() << "ISD: WrapperLarge\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::TLSDESC_CALLSEQ: {
            DEBUG(errs() << "ISD: TLSDESC_CALLSEQ\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ADRP: {
            uint64_t Offset = cast<ConstantInt>(getNextOperand())->getValue().getZExtValue();
            registerResult(Builder->getInt64((CurrentInst->Address + Offset) & ~0xFFF));
            DEBUG(errs() << "ISD: ADRP\n");
            break;
        }
        case AArch64ISD::ADDlow: {
            DEBUG(errs() << "ISD: ADDlow\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LOADgot: {
            DEBUG(errs() << "ISD: LOADgot\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::RET_FLAG: {
            DEBUG(errs() << "ISD: RET_FLAG\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::CSEL: {
            DEBUG(errs() << "ISD: CSEL\n");
            Value *V1 = getNextOperand();
            Value *V2 = getNextOperand();
            Value *V3 = getNextOperand();
            Value *NZCV = getNextOperand();
            registerResult(Builder->CreateSelect(V3, V1, V2));
            break;
        }
        case AArch64ISD::FCSEL: {
            DEBUG(errs() << "ISD: FCSEL\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::CSINV: {
            DEBUG(errs() << "ISD: CSINV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::CSNEG: {
            DEBUG(errs() << "ISD: CSNEG\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::CSINC: {
            DEBUG(errs() << "ISD: CSINC\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::THREAD_POINTER: {
            DEBUG(errs() << "ISD: THREAD_POINTER\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ADC: {
            DEBUG(errs() << "ISD: ADC\n");
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *nzcv = getNextOperand();

            Value *one = Builder->getInt(APInt(op1->getType()->getPrimitiveSizeInBits(), 1));
            Value *C_flag = Builder->CreateAnd(nzcv, Builder->getInt32(0x1 << AArch64::NZCVShift::C));

            C_flag = Builder->CreateZExtOrTrunc(C_flag, op2->getType());

            op2 = Builder->CreateSub(op2, one);
            op2 = Builder->CreateAdd(op2, C_flag);
            Value *Result = Builder->CreateSub(op1, op2);

            registerResult(Result);
            break;
        }
        case AArch64ISD::SBC: {
            DEBUG(errs() << "ISD: SBC\n");
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *nzcv = getNextOperand();

            Value *one = Builder->getInt(APInt(op1->getType()->getPrimitiveSizeInBits(), 1));
            Value *C_flag = Builder->CreateAnd(nzcv, Builder->getInt32(0x1 << AArch64::NZCVShift::C));

            C_flag = Builder->CreateZExtOrTrunc(C_flag, op2->getType());

            op2 = Builder->CreateSub(op2, one);
            op2 = Builder->CreateAdd(op2, C_flag);
            Value *Result = Builder->CreateSub(op1, op2);

            registerResult(Result);
            break;
        }
        case AArch64ISD::ADCS: {
            DEBUG(errs() << "ISD: ADCS\n");
            EVT res2VT = NextVT();
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *nzcv = getNextOperand();

            Value *C_flag = Builder->CreateAnd(nzcv, Builder->getInt32(0x1 << AArch64::NZCVShift::C));

            C_flag = Builder->CreateZExtOrTrunc(C_flag, op2->getType());

            op2 = Builder->CreateAdd(op2, C_flag);
            Value *Result = Builder->CreateAdd(op1, op2);

            Value *nzcvNew = getNZCVFlags(Result, op1, op2);

            registerResult(Result);
            registerResult(nzcvNew);
            break;
        }
        case AArch64ISD::SBCS: {
            DEBUG(errs() << "ISD: SBCS\n");
            EVT res2VT = NextVT();
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *nzcv = getNextOperand();

            Value *one = Builder->getInt(APInt(op1->getType()->getPrimitiveSizeInBits(), 1));
            Value *C_flag = Builder->CreateAnd(nzcv, Builder->getInt32(0x1 << AArch64::NZCVShift::C));

            C_flag = Builder->CreateZExtOrTrunc(C_flag, op2->getType());

            op2 = Builder->CreateSub(op2, one);
            op2 = Builder->CreateAdd(op2, C_flag);
            Value *Result = Builder->CreateSub(op1, op2);

            Value *nzcvNew = getNZCVFlags(Result, op1, op2);

            registerResult(Result);
            registerResult(nzcvNew);
            break;
        }
        case AArch64ISD::ANDS: {
            DEBUG(errs() << "ISD: ANDS\n");
            NextVT();
            Value *LHS = getNextOperand();
            Value *RHS = getNextOperand();
            Value *Result = Builder->CreateAnd(LHS, RHS);
            registerResult(Result);
            registerResult(getNZCVFlags(Result));
            break;
        }
        case AArch64ISD::CCMN: {
            DEBUG(errs() << "ISD: CCMN\n");

            Value *LHS = getNextOperand();
            Value *RHS = getNextOperand();
            Value *NZCVfalse = getNextOperand();
            NZCVfalse = Builder->CreateShl(NZCVfalse, 28);
            Value *Cond = getNextOperand();

            //Old NZCV not used...
            (void)getNextOperand();

            Value *Minus_One = ConstantInt::get(RHS->getType(), -1);
            RHS = Builder->CreateMul(RHS, Minus_One);
            Value *Result = Builder->CreateSub(LHS, RHS);
            Value *NZCVtrue = getNZCVFlags(Result, LHS, RHS);

            registerResult(Builder->CreateSelect(Cond, NZCVtrue, NZCVfalse));
            break;
        }
        case AArch64ISD::FCCMP: {
            DEBUG(errs() << "ISD: FCCMP\n");
            Value *Op1 = getNextOperand();
            Value *Op2 = getNextOperand();
            Value *NZCVfalse = getNextOperand();
            NZCVfalse = Builder->CreateShl(NZCVfalse, 28);
            Value *Cond = getNextOperand();
            //Old NZCV is not used
            (void)getNextOperand();
            Value *NZCVtrue = FPCompare(Op1, Op2);
            registerResult(Builder->CreateSelect(Cond, NZCVtrue, NZCVfalse));
            break;
        }
        case AArch64ISD::FCMP: {
            DEBUG(errs() << "ISD: FCMP\n");
            Value *Op1 = getNextOperand();
            Value *Op2 = getNextOperand();
            registerResult(FPCompare(Op1, Op2));
            break;
        }
        case AArch64ISD::EXTR: {
            DEBUG(errs() << "ISD: EXTR\n");
            //FIXME: EXTR is handled wrong for sure...
            Value *Reg1 = getNextOperand();
            Value *Reg2 = getNextOperand();
            ConstantInt *lsbVal = dyn_cast<ConstantInt>(getNextOperand());
            Value *Result = Builder->CreateOr(Reg1, Reg2);
            registerResult(Result);
            break;
        }
        case AArch64ISD::DUP: {
            DEBUG(errs() << "ISD: DUP\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *Op = getNextOperand();
            MVT SVT = ResEVT.getSimpleVT();

            Type *ElemType = IntegerType::get(*Ctx, SVT.getScalarSizeInBits());
            Value *Elem = Builder->CreateTrunc(Op, ElemType);

            Value *Vector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
            Elem = Builder->CreateZExt(Elem, Vector->getType());

            for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                Vector = Builder->CreateOr(Vector, Elem);
                Elem = Builder->CreateShl(Elem, SVT.getScalarSizeInBits());
            }

            Vector = Builder->CreateBitCast(Vector, ResEVT.getTypeForEVT(*Ctx));
            registerResult(Vector);

            break;
        }

        case AArch64ISD::DUPLANE16:
        case AArch64ISD::DUPLANE32:
        case AArch64ISD::DUPLANE8:
        case AArch64ISD::DUPLANE64: {
            switch (Opcode) {
                case AArch64ISD::DUPLANE8:
                    DEBUG(errs() << "ISD: DUPLANE8\n");
                    break;
                case AArch64ISD::DUPLANE16:
                    DEBUG(errs() << "ISD: DUPLANE16\n");
                    break;
                case AArch64ISD::DUPLANE32:
                    DEBUG(errs() << "ISD: DUPLANE32\n");
                    break;
                case AArch64ISD::DUPLANE64:
                    DEBUG(errs() << "ISD: DUPLANE64\n");
                    break;
            }
            assert(ResEVT.getSimpleVT().isVector());

            Value *SrcVector = getNextOperand();
            Value *Idx = getNextOperand();
            Value *Elem = Builder->CreateExtractElement(SrcVector, Idx);

            Value *Vector = Builder->getInt(APInt(ResEVT.getSimpleVT().getVectorNumElements() * ResEVT.getSimpleVT().getScalarSizeInBits(), 0));
            Vector = Builder->CreateBitCast(Vector, VectorType::get(Elem->getType(), ResEVT.getSimpleVT().getVectorNumElements()));
            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Vector = Builder->CreateInsertElement(Vector, Elem, i);
            }
            registerResult(Builder->CreateBitCast(Vector, ResEVT.getTypeForEVT(*Ctx)));

            break;
        }
        case AArch64ISD::MOVIedit:
        case AArch64ISD::MOVI: {
            MVT SVT = ResEVT.getSimpleVT();
            if (SVT.isVector()) {
                Value *V = getNextOperand();
                if (ConstantInt *CI = dyn_cast<ConstantInt>(V)) {
                    uint64_t Const = CI->getValue().getZExtValue();
                    Value *ConstVal = Builder->getInt(APInt(SVT.getScalarSizeInBits(), Const));
                    ConstVal = Builder->CreateZExt(ConstVal, IntegerType::get(*Ctx, SVT.getVectorNumElements() * SVT.getScalarSizeInBits()));
                    Value *Vector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
                    for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                        Vector = Builder->CreateOr(Vector, ConstVal);
                        ConstVal = Builder->CreateShl(ConstVal, SVT.getScalarSizeInBits());
                    }
                    registerResult(Vector);


//                    Value *Vector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
//                    Vector = Builder->CreateBitCast(Vector, VectorType::get(IntegerType::get(*Ctx, SVT.getScalarSizeInBits()), SVT.getVectorNumElements()));
//                    for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
//                        Builder->CreateInsertElement(Vector, Builder->getInt(APInt(SVT.getScalarSizeInBits(), Const)), i);
//                    }
//                    Vector = Builder->CreateBitCast(Vector, IntegerType::get(*Ctx, SVT.getScalarSizeInBits() * SVT.getVectorNumElements()));
//                    registerResult(Vector);
                } else {

                    Value *ConstVal = Builder->CreateSExtOrTrunc(V, IntegerType::get(*Ctx, ResEVT.getScalarSizeInBits()));
                    ConstVal = Builder->CreateZExt(ConstVal, IntegerType::get(*Ctx, SVT.getVectorNumElements() * SVT.getScalarSizeInBits()));
                    Value *Vector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
                    for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                        Vector = Builder->CreateOr(Vector, ConstVal);
                        ConstVal = Builder->CreateShl(ConstVal, SVT.getScalarSizeInBits());
                    }
                    registerResult(Vector);
                }
            } else {
                llvm_unreachable("MVT not implemented!");
            }
            DEBUG(errs() << "ISD: MOVI\n");
            break;
        }
        case AArch64ISD::MOVIshift: {
            DEBUG(errs() << "ISD: MOVIshift\n");
            Value *imm = getNextOperand();
            Value *shift = getNextOperand();
            imm = Builder->CreateShl(imm, shift);
            imm = Builder->CreateZExtOrTrunc(imm, ResEVT.getVectorElementType().getTypeForEVT(*Ctx));

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
            for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
                result = Builder->CreateInsertElement(result, imm, i);
            }
            registerResult(result);
            break;
        }
//        case AArch64ISD::MOVIedit: {
//            errs() << "ISD: MOVIedit\n";
//            llvm_unreachable("Not implemented");
//            break;
//        }
        case AArch64ISD::MOVImsl: {
            DEBUG(errs() << "ISD: MOVImsl\n");
            Value *imm = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);
            imm = Builder->CreateShl(imm, shift);
            imm = Builder->CreateZExtOrTrunc(imm, ResEVT.getVectorElementType().getTypeForEVT(*Ctx));
            uint64_t lo = getOnes(shift->getZExtValue());
            imm = Builder->CreateOr(imm, lo);

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
            for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
                result = Builder->CreateInsertElement(result, imm, i);
            }
            registerResult(result);
            break;
        }
        case AArch64ISD::FMOV: {
            DEBUG(errs() << "ISD: FMOV\n");

            if (ResEVT.getSimpleVT().isVector()) {
                Value *Op = getNextOperand();
                MVT SVT = ResEVT.getSimpleVT();

                Type *ElemType = IntegerType::get(*Ctx, SVT.getVectorNumElements() * SVT.getScalarSizeInBits());
                Value *Elem = Builder->CreateZExtOrTrunc(Op, ElemType);

                Value *Vector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));

                for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                    Vector = Builder->CreateOr(Vector, Elem);
                    Elem = Builder->CreateShl(Elem, SVT.getScalarSizeInBits());
                }

                Vector = Builder->CreateBitCast(Vector, ResEVT.getTypeForEVT(*Ctx));
                registerResult(Vector);
            } else {
                llvm_unreachable("has to be a vector?");
            }

            break;
        }
        case AArch64ISD::MVNIshift: {
            DEBUG(errs() << "ISD: MVNIshift\n");
            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));

            Value *elem = getNextOperand();
            Value *shift = getNextOperand();
            elem = Builder->CreateShl(elem, shift);
            elem = Builder->CreateZExtOrTrunc(elem, ResEVT.getVectorElementType().getTypeForEVT(*Ctx));

            for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
                result = Builder->CreateInsertElement(result, elem, i);
            }
            registerResult(result);
            break;
        }
        case AArch64ISD::MVNImsl: {
            DEBUG(errs() << "ISD: MVNImsl\n");
            Value *imm = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);
            imm = Builder->CreateShl(imm, shift);
            imm = Builder->CreateNot(imm);
            imm = Builder->CreateZExtOrTrunc(imm, ResEVT.getVectorElementType().getTypeForEVT(*Ctx));
            uint64_t lo = getOnes(shift->getZExtValue());
            imm = Builder->CreateOr(imm, lo);

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
            for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
                result = Builder->CreateInsertElement(result, imm, i);
            }
            registerResult(result);
            break;
        }
        case AArch64ISD::BICi: {
            DEBUG(errs() << "ISD: BICi\n");
            Value *vec = getNextOperand();
            VectorType *vecTy = dyn_cast<VectorType>(vec->getType());
            assert(vecTy);
            Value *mask = getNextOperand();
            Value *shift = getNextOperand();

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));

            if (VectorType *maskTy = dyn_cast<VectorType>(mask->getType())) {
                llvm_unreachable("not implemented yet..");
            } else {
                for (unsigned i = 0; i < vecTy->getVectorNumElements(); ++i) {
                    Value *elem = Builder->CreateNot(Builder->CreateShl(mask, shift));
                    elem = Builder->CreateZExtOrTrunc(elem, vecTy->getElementType());
                    elem = Builder->CreateAnd(Builder->CreateExtractElement(vec, i), elem);
                    result = Builder->CreateInsertElement(result, elem, i);
                }
            }
            registerResult(result);
            break;
        }
        case AArch64ISD::ORRi: {
            DEBUG(errs() << "ISD: ORRi\n");
            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));

            Value *vec = getNextOperand();

            Value *val = getNextOperand();
            val = Builder->CreateShl(val, getNextOperand());
            val = Builder->CreateZExtOrTrunc(val, ResEVT.getVectorElementType().getTypeForEVT(*Ctx));

            for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
                Value *elem = Builder->CreateOr(Builder->CreateExtractElement(vec, i), val);
                result = Builder->CreateInsertElement(result, elem, i);
            }

            registerResult(result);

            break;
        }
        case AArch64ISD::BSL: {
            DEBUG(errs() << "ISD: BSL\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::NEG: {
            DEBUG(errs() << "ISD: NEG\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ZIP1: {
            DEBUG(errs() << "ISD: ZIP1\n");
            //FIXME: not handled correct...
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *result = Builder->CreateOr(op1, op2);
            registerResult(result);
            break;
        }
        case AArch64ISD::ZIP2: {
            DEBUG(errs() << "ISD: ZIP2\n");
            //FIXME: not handled correct...
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *result = Builder->CreateOr(op1, op2);
            registerResult(result);
            break;
        }
        case AArch64ISD::UZP1: {
            DEBUG(errs() << "ISD: UZP1\n");
            //FIXME: not handled correct...
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *result = Builder->CreateOr(op1, op2);
            registerResult(result);
            break;
        }
        case AArch64ISD::UZP2: {
            DEBUG(errs() << "ISD: UZP2\n");
            //FIXME: not handled correct...
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *result = Builder->CreateOr(op1, op2);
            registerResult(result);
            break;
        }
        case AArch64ISD::TRN1: {
            DEBUG(errs() << "ISD: TRN1\n");
            //FIXME: implement this!
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *result = Builder->CreateOr(op1, op2);
            registerResult(result);
            break;
        }
        case AArch64ISD::TRN2: {
            DEBUG(errs() << "ISD: TRN2\n");
            //FIXME: implement this!
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *result = Builder->CreateOr(op1, op2);
            registerResult(result);
            break;
        }
        case AArch64ISD::REV16: {
            DEBUG(errs() << "ISD: REV16\n");
            //llvm_unreachable("Not implemented");
            //BugID: vlc_0x100C6D7D0
            Value *in = getNextOperand();
            in = Builder->CreateBitCast(in, VectorType::get(Builder->getInt16Ty(), ResEVT.getSizeInBits() / 16));

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, VectorType::get(Builder->getInt16Ty(), ResEVT.getSizeInBits() / 16));

            std::vector<Type*> types;
            types.push_back(Builder->getInt16Ty());

            Function *intr = Intrinsic::getDeclaration(TheModule, Intrinsic::aarch64_rbit, types);

            for (unsigned i = 0; i < ResEVT.getSizeInBits() / 2; ++i) {
                std::vector<Value*> args;
                args.push_back(Builder->CreateExtractElement(in, i));
                Value *rev = Builder->CreateCall(intr, args);
                result = Builder->CreateInsertElement(result, rev, i);
            }
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
            registerResult(result);
            break;
        }
        case AArch64ISD::REV32: {
            DEBUG(errs() << "ISD: REV32\n");
            // TODO: Check Implementation. Is this correct?
            // Two possibilites here:
            // REV32 Vd.<T>, Vn.<T> => Element reverse in 32-bit words (vector). Where <T> is 8B, 16B, 4H, or 8H.
            // REV32 Xd, Xm => Reverse Bytes in Words (extended)
            //llvm_unreachable("Not implemented");
            Value *in = getNextOperand();
            in = Builder->CreateBitCast(in, VectorType::get(Builder->getInt32Ty(), ResEVT.getSizeInBits() / 32));

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, VectorType::get(Builder->getInt32Ty(), ResEVT.getSizeInBits() / 32));

            std::vector<Type*> types;
            types.push_back(Builder->getInt32Ty());

            Function *intr = Intrinsic::getDeclaration(TheModule, Intrinsic::aarch64_rbit, types);

            for (unsigned i = 0; i < ResEVT.getSizeInBits() / 2; ++i) {
                std::vector<Value*> args;
                args.push_back(Builder->CreateExtractElement(in, i));
                Value *rev = Builder->CreateCall(intr, args);
                result = Builder->CreateInsertElement(result, rev, i);
            }
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
            registerResult(result);
            break;
        }
        case AArch64ISD::REV64: {
            DEBUG(errs() << "ISD: REV64\n");
            Value *in = getNextOperand();
            in = Builder->CreateBitCast(in, VectorType::get(Builder->getInt64Ty(), ResEVT.getSizeInBits() / 64));

            Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, VectorType::get(Builder->getInt64Ty(), ResEVT.getSizeInBits() / 64));

            std::vector<Type*> types;
            types.push_back(Builder->getInt64Ty());

            Function *intr = Intrinsic::getDeclaration(TheModule, Intrinsic::aarch64_rbit, types);

            for (unsigned i = 0; i < ResEVT.getSizeInBits() / 2; ++i) {
                std::vector<Value*> args;
                args.push_back(Builder->CreateExtractElement(in, i));
                Value *rev = Builder->CreateCall(intr, args);
                result = Builder->CreateInsertElement(result, rev, i);
            }
            result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));
            registerResult(result);
            break;
        }
        case AArch64ISD::EXT: {
            DEBUG(errs() << "ISD: EXT\n");
            //FIXME: Not implemented yet!!!!!!!!!!!
            Value *V1 = getNextOperand();
            Value *V2 = getNextOperand();
            Value *V3 = getNextOperand();
            registerResult(V1);
            break;
        }
        case AArch64ISD::VSHL: {
            DEBUG(errs() << "ISD: VSHL\n");
            Value *Vector = getNextOperand();
            Value *SHRVal = getNextOperand();
            uint64_t SHL = cast<ConstantInt>(SHRVal)->getValue().getZExtValue();
            Vector = Builder->CreateShl(Vector, SHL);
            registerResult(Vector);
            break;
        }
        case AArch64ISD::VLSHR: {
            DEBUG(errs() << "ISD: VLSHR\n");
            Value *Vector = getNextOperand();
            Value *SHRVal = getNextOperand();
            uint64_t SHR = cast<ConstantInt>(SHRVal)->getValue().getZExtValue();
            Vector = Builder->CreateLShr(Vector, SHR);
            registerResult(Vector);
            break;
        }
        case AArch64ISD::VASHR: {
            DEBUG(errs() << "ISD: VASHR\n");
            //assert(ResEVT.getSimpleVT().isVector());
            if (!ResEVT.getSimpleVT().isVector()) {
                Value *Vector = getNextOperand();
                Value *SHRVal = getNextOperand();
                uint64_t SHR = cast<ConstantInt>(SHRVal)->getValue().getZExtValue();
                Vector = Builder->CreateAShr(Vector, SHR);
                registerResult(Vector);
                break;
            }
            Value *op = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);
            Value *result = Builder->getInt(APInt(ResEVT.getSimpleVT().getVectorNumElements() * ResEVT.getSimpleVT().getScalarSizeInBits(), 0));
            result = Builder->CreateBitCast(result, op->getType());
            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *elem = Builder->CreateExtractElement(op, i);
                elem = Builder->CreateAShr(elem, shift->getZExtValue());
                result = Builder->CreateInsertElement(result, elem, i);
            }
            registerResult(result);
            break;
        }
        //modified
        case AArch64ISD::SQSHL_I: 
        //    DEBUG(errs() << "ISD: SQSHL_I\n");
        //    llvm_unreachable("Not implemented");
        //    break;
        //}
        //BugID: koubei_100BF4608
        case AArch64ISD::UQSHL_I: {
            DEBUG(errs() << "ISD: UQSHL_I\n");
            //llvm_unreachable("Not implemented");
            Value *op1 = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);

            Value *result = Builder->getInt(APInt(ResEVT.getSimpleVT().getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, op1->getType());

            if (!ResEVT.getSimpleVT().isVector()) {
                Value *result = Builder->CreateShl(op1, shift->getZExtValue());
                registerResult(result);
                break;
            }

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *elem = Builder->CreateExtractElement(op1, i);
                elem = Builder->CreateShl(elem, shift->getZExtValue());
                result = Builder->CreateInsertElement(result, elem, i);
            }

            registerResult(result);

            break;
            break;
        }
        //modified
        case AArch64ISD::SQSHLU_I: {
            DEBUG(errs() << "ISD: SQSHLU_I\n");
            //llvm_unreachable("Not implemented");
            Value *op1 = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);

            Value *result = Builder->getInt(APInt(ResEVT.getSimpleVT().getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, op1->getType());

            if (!ResEVT.getSimpleVT().isVector()) {
                Value *result = Builder->CreateShl(op1, shift->getZExtValue());
                registerResult(result);
                break;
            }

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *elem = Builder->CreateExtractElement(op1, i);
                elem = Builder->CreateShl(elem, shift->getZExtValue());
                result = Builder->CreateInsertElement(result, elem, i);
            }

            registerResult(result);
            break;
        }
        case AArch64ISD::SRSHR_I: {
            DEBUG(errs() << "ISD: SRSHR_I\n");
            Value *op1 = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);

            Value *result = Builder->getInt(APInt(ResEVT.getSimpleVT().getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, op1->getType());

            if (!ResEVT.getSimpleVT().isVector()) {
                Value *result = Builder->CreateAShr(op1, shift->getZExtValue());
                registerResult(result);
                break;
            }

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *elem = Builder->CreateExtractElement(op1, i);
                elem = Builder->CreateAShr(elem, shift->getZExtValue());
                result = Builder->CreateInsertElement(result, elem, i);
            }

            registerResult(result);

            break;
        }
        //BugID: vlc_100C6C330
        case AArch64ISD::URSHR_I: {
            DEBUG(errs() << "ISD: URSHR_I\n");
            //llvm_unreachable("Not implemented");
            Value *op1 = getNextOperand();
            ConstantInt *shift = dyn_cast<ConstantInt>(getNextOperand());
            assert(shift);

            Value *result = Builder->getInt(APInt(ResEVT.getSimpleVT().getSizeInBits(), 0));
            result = Builder->CreateBitCast(result, op1->getType());

            if (!ResEVT.getSimpleVT().isVector()) {
                Value *result = Builder->CreateLShr(op1, shift->getZExtValue());
                registerResult(result);
                break;
            }

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *elem = Builder->CreateExtractElement(op1, i);
                elem = Builder->CreateLShr(elem, shift->getZExtValue());
                result = Builder->CreateInsertElement(result, elem, i);
            }

            registerResult(result);
            break;
        }
        case AArch64ISD::CMEQ: {
            DEBUG(errs() << "ISD: CMEQ\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();
            Value *vector2 = getNextOperand();

            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++ i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->CreateExtractElement(vector2, i);
                Value *cmp = Builder->CreateICmpEQ(elem1, elem2);

                Value *v = Builder->CreateSelect(cmp, Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), ones)), Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0)));
                Builder->CreateInsertElement(Result, v, i);
            }
            registerResult(Result);
            break;
        }
        case AArch64ISD::CMGE: {
            DEBUG(errs() << "ISD: CMGE\n");
            //llvm_unreachable("Not implemented");
            MVT SVT = ResEVT.getSimpleVT();

            Value *Vector1 = getNextOperand();
            Value *Vector2 = getNextOperand();

            Value *ResultVector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
            ResultVector = Builder->CreateBitCast(ResultVector, VectorType::get(IntegerType::get(*Ctx, SVT.getScalarSizeInBits()), SVT.getVectorNumElements()));

            Value *OneMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), -1U));
            Value *ZeroMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), 0));
            for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                Value *E1 = Builder->CreateExtractElement(Vector1, i);
                Value *E2 = Builder->CreateExtractElement(Vector2, i);

                Value *NewElem = Builder->CreateSelect(Builder->CreateICmpSGE(E1, E2), OneMask, ZeroMask);
                ResultVector = Builder->CreateInsertElement(ResultVector, NewElem, i);
            }

            registerResult(ResultVector);
            break;
        }
        case AArch64ISD::CMGT: {
            DEBUG(errs() << "ISD: CMGT\n");
            MVT SVT = ResEVT.getSimpleVT();

            Value *Vector1 = getNextOperand();
            Value *Vector2 = getNextOperand();

            Value *ResultVector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
            ResultVector = Builder->CreateBitCast(ResultVector, VectorType::get(IntegerType::get(*Ctx, SVT.getScalarSizeInBits()), SVT.getVectorNumElements()));

            Value *OneMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), -1U));
            Value *ZeroMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), 0));
            for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                Value *E1 = Builder->CreateExtractElement(Vector1, i);
                Value *E2 = Builder->CreateExtractElement(Vector2, i);

                Value *NewElem = Builder->CreateSelect(Builder->CreateICmpSGT(E1, E2), OneMask, ZeroMask);
                ResultVector = Builder->CreateInsertElement(ResultVector, NewElem, i);
            }

            registerResult(ResultVector);
            break;
        }
        case AArch64ISD::CMHI: {
            DEBUG(errs() << "ISD: CMHI\n");
            MVT SVT = ResEVT.getSimpleVT();

            Value *Vector1 = getNextOperand();
            Value *Vector2 = getNextOperand();

            Value *ResultVector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
            ResultVector = Builder->CreateBitCast(ResultVector, VectorType::get(IntegerType::get(*Ctx, SVT.getScalarSizeInBits()), SVT.getVectorNumElements()));

            Value *OneMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), -1U));
            Value *ZeroMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), 0));
            for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                Value *E1 = Builder->CreateExtractElement(Vector1, i);
                Value *E2 = Builder->CreateExtractElement(Vector2, i);

                Value *NewElem = Builder->CreateSelect(Builder->CreateICmpUGT(E1, E2), OneMask, ZeroMask);
                ResultVector = Builder->CreateInsertElement(ResultVector, NewElem, i);
            }

            registerResult(ResultVector);
            break;
        }
        case AArch64ISD::CMHS: {
            DEBUG(errs() << "ISD: CMHS\n");
            MVT SVT = ResEVT.getSimpleVT();

            Value *Vector1 = getNextOperand();
            Value *Vector2 = getNextOperand();

            Value *ResultVector = Builder->getInt(APInt(SVT.getVectorNumElements() * SVT.getScalarSizeInBits(), 0));
            ResultVector = Builder->CreateBitCast(ResultVector, VectorType::get(IntegerType::get(*Ctx, SVT.getScalarSizeInBits()), SVT.getVectorNumElements()));

            Value *OneMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), -1U));
            Value *ZeroMask = Builder->getInt(APInt(SVT.getScalarSizeInBits(), 0));
            for (unsigned i = 0; i < SVT.getVectorNumElements(); ++i) {
                Value *E1 = Builder->CreateExtractElement(Vector1, i);
                Value *E2 = Builder->CreateExtractElement(Vector2, i);

                Value *NewElem = Builder->CreateSelect(Builder->CreateICmpSGT(E1, E2), OneMask, ZeroMask);
                ResultVector = Builder->CreateInsertElement(ResultVector, NewElem, i);
            }

            registerResult(ResultVector);
            break;
        }
        case AArch64ISD::FCMEQ:
        case AArch64ISD::FCMGE:
        case AArch64ISD::FCMGT:
        case AArch64ISD::FCMEQz:
        case AArch64ISD::FCMGEz:
        case AArch64ISD::FCMGTz:
        case AArch64ISD::FCMLEz:
        case AArch64ISD::FCMLTz: {
            Value *op1 = getNextOperand();
            Value *op2 = nullptr;

            switch (Opcode) {
                case AArch64ISD::FCMEQ:
                case AArch64ISD::FCMGE:
                case AArch64ISD::FCMGT:
                {
                    op2 = getNextOperand();
                    break;
                }
                case AArch64ISD::FCMEQz:
                case AArch64ISD::FCMGEz:
                case AArch64ISD::FCMGTz:
                case AArch64ISD::FCMLEz:
                case AArch64ISD::FCMLTz:
                {
                    op2 = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
                    op2 = Builder->CreateBitCast(op2, op1->getType());
                    break;
                }
            }

            if (ResEVT.isVector()) {
                Type *elemTy = ResEVT.getVectorElementType().getTypeForEVT(*Ctx);
                Constant *zero = ConstantInt::get(elemTy, 0, false);
                Constant *allOnes = ConstantInt::get(elemTy, -1U, false);

                Value *result = Builder->getInt(APInt(ResEVT.getSizeInBits(), 0));
                result = Builder->CreateBitCast(result, ResEVT.getTypeForEVT(*Ctx));

                for (unsigned i = 0; i < ResEVT.getVectorNumElements(); ++i) {
                    Value *elem1 = Builder->CreateExtractElement(op1, i);
                    Value *elem2 = Builder->CreateExtractElement(op2, i);

                    Value *cmp = nullptr;
                    switch (Opcode) {
                        default:
                            llvm_unreachable("");
                        case AArch64ISD::FCMGTz:
                        case AArch64ISD::FCMGT:
                        {
                            DEBUG(errs() << "ISD: FCMGT\n");
                            cmp = Builder->CreateFCmpUGT(elem1, elem2);
                            break;
                        }
                        case AArch64ISD::FCMGEz:
                        case AArch64ISD::FCMGE:
                        {
                            DEBUG(errs() << "ISD: FCMGE\n");
                            cmp = Builder->CreateFCmpUGE(elem1, elem2);
                            break;
                        }
                        case AArch64ISD::FCMEQz:
                        case AArch64ISD::FCMEQ: {
                            DEBUG(errs() << "ISD: FCMEQ\n");
                            cmp = Builder->CreateFCmpUEQ(elem1, elem2);
                            break;
                        }
                        case AArch64ISD::FCMLEz:
                        {
                            DEBUG(errs() << "ISD: FCMLEz\n");
                            cmp = Builder->CreateFCmpULE(elem1, elem2);
                            break;
                        }
                        case AArch64ISD::FCMLTz:
                        {
                            DEBUG(errs() << "ISD: FCMLTz\n");
                            cmp = Builder->CreateFCmpULT(elem1, elem2);
                            break;
                        }
                    }

                    Value *resElem = Builder->CreateSelect(cmp, allOnes, zero);
                    result = Builder->CreateInsertElement(result, resElem, i);
                }

                registerResult(result);

            } else {
                llvm_unreachable("");
            }

            break;
        }
        case AArch64ISD::CMEQz: {
            DEBUG(errs() << "ISD: CMEQz\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();

            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++ i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0));
                Value *cmp = Builder->CreateICmpEQ(elem1, elem2);

                Value *v = Builder->CreateSelect(cmp, Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), ones)), Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0)));
                Builder->CreateInsertElement(Result, v, i);
            }
            registerResult(Result);
            break;
        }
        case AArch64ISD::CMGEz: {
            DEBUG(errs() << "ISD: CMGEz\n");
            //llvm_unreachable("Not implemented");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();

            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++ i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0));
                Value *cmp = Builder->CreateICmpSGE(elem1, elem2);

                Value *v = Builder->CreateSelect(cmp, Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), ones)), Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0)));
                Builder->CreateInsertElement(Result, v, i);
            }
            registerResult(Result);
            break;
        }
        case AArch64ISD::CMGTz: {
            DEBUG(errs() << "ISD: CMGTz\n");
            //llvm_unreachable("Not implemented");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();

            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++ i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0));
                Value *cmp = Builder->CreateICmpSGT(elem1, elem2);

                Value *v = Builder->CreateSelect(cmp, Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), ones)), Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0)));
                Builder->CreateInsertElement(Result, v, i);
            }
            registerResult(Result);
            break;
        }
        case AArch64ISD::CMLEz: {
            DEBUG(errs() << "ISD: CMLEz\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();

            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++ i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0));
                Value *cmp = Builder->CreateICmpSLE(elem1, elem2);

                Value *v = Builder->CreateSelect(cmp, Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), ones)), Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0)));
                Builder->CreateInsertElement(Result, v, i);
            }
            registerResult(Result);
            break;
        }
        case AArch64ISD::CMLTz: {
            DEBUG(errs() << "ISD: CMLTz\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();

            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));

            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++ i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0));
                Value *cmp = Builder->CreateICmpSLT(elem1, elem2);

                Value *v = Builder->CreateSelect(cmp, Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), ones)), Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), 0)));
                Builder->CreateInsertElement(Result, v, i);
            }
            registerResult(Result);
            break;
        }
        case AArch64ISD::SADDV: {
            DEBUG(errs() << "ISD: SADDV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::UADDV: {
            DEBUG(errs() << "ISD: UADDV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::SMINV: {
            DEBUG(errs() << "ISD: SMINV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::UMINV: {
            DEBUG(errs() << "ISD: UMINV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::SMAXV: {
            DEBUG(errs() << "ISD: SMAXV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::UMAXV: {
            DEBUG(errs() << "ISD: UMAXV\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::NOT: {
            DEBUG(errs() << "ISD: NOT\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *vector1 = getNextOperand();
            
            //vector1 = Builder->CreateNeg(vector1);
            
            uint64_t ones = getOnes(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits());

            Value *Result = UndefValue::get(VectorType::get(IntegerType::get(*Ctx, ResEVT.getVectorElementType().getSizeInBits()), ResEVT.getVectorNumElements()));
            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *elem1 = Builder->CreateExtractElement(vector1, i);
                Value *elem2 = Builder->getInt(APInt(ResEVT.getVectorElementType().getSizeInBits(), -1));
                Value *neg = Builder->CreateNeg(elem1);

                Builder->CreateInsertElement(Result, neg, i);
            }

            registerResult(Result);
            //llvm_unreachable("Not implemented");

            break;
        }
        case AArch64ISD::BIT: {
            DEBUG(errs() << "ISD: BIT\n");
            assert(ResEVT.getSimpleVT().isVector());

            Value *Result = getNextOperand();

            Value *ValueVec = getNextOperand();
            ValueVec = Builder->CreateBitCast(ValueVec, ResEVT.getTypeForEVT(*Ctx));
            Value *cmpVec = getNextOperand();
            cmpVec = Builder->CreateBitCast(cmpVec, ResEVT.getTypeForEVT(*Ctx));

            Value *zero = Builder->getInt(APInt(ResEVT.getSimpleVT().getVectorElementType().getSizeInBits(), 0));
            for (unsigned i = 0; i < ResEVT.getSimpleVT().getVectorNumElements(); ++i) {
                Value *cmpElement = Builder->CreateExtractElement(cmpVec, i);
                Value *cmp = Builder->CreateICmpEQ(cmpElement, zero);
                Value *trueElem = Builder->CreateExtractElement(ValueVec, i);
                Value *v = Builder->CreateSelect(cmp, zero, trueElem);
                Result = Builder->CreateInsertElement(Result, v, i);
            }

            registerResult(Result);
            break;
        }
        case AArch64ISD::TBZ: {
            DEBUG(errs() << "ISD: TBZ\n");
            Value *CmpReg = getNextOperand();
            Value *Mask = getNextOperand();
            Value *TrueTargetOffset = getNextOperand();

            Value *Cmp = Builder->CreateAnd(CmpReg, Mask);
            Cmp = Builder->CreateICmpEQ(Cmp, ConstantInt::get(cast<IntegerType>(CmpReg->getType()), 0));

            uint64_t Target = cast<ConstantInt>(TrueTargetOffset)->getValue().getZExtValue();
            BasicBlock *TrueBBlock = getOrCreateBasicBlock(Target);
            BasicBlock *FalseBBlock = getOrCreateBasicBlock(getBasicBlockEndAddress());
            Builder->CreateCondBr(Cmp, TrueBBlock, FalseBBlock);
            break;
        }
        case AArch64ISD::TBNZ: {
            DEBUG(errs() << "ISD: TBNZ\n");
            Value *CmpReg = getNextOperand();
            Value *Mask = getNextOperand();
            Value *TrueTargetOffset = getNextOperand();

            Value *Cmp = Builder->CreateAnd(CmpReg, Mask);
            Cmp = Builder->CreateICmpNE(Cmp, ConstantInt::get(cast<IntegerType>(CmpReg->getType()), 0));

            uint64_t Target = cast<ConstantInt>(TrueTargetOffset)->getValue().getZExtValue();
            BasicBlock *TrueBBlock = getOrCreateBasicBlock(Target);
            BasicBlock *FalseBBlock = getOrCreateBasicBlock(getBasicBlockEndAddress());
            Builder->CreateCondBr(Cmp, TrueBBlock, FalseBBlock);
            break;
        }
        case AArch64ISD::TC_RETURN: {
            DEBUG(errs() << "ISD: TC_RETURN\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::PREFETCH: {
            DEBUG(errs() << "ISD: PREFETCH\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::SITOF: {
            DEBUG(errs() << "ISD: SITOF\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Op = getNextOperand();
            if (!Op->getType()->isIntegerTy()) {
                Op = Builder->CreateBitCast(Op, IntegerType::get(getGlobalContext(), ResType->getScalarSizeInBits()));
            }
            registerResult(Builder->CreateSIToFP(Op, ResType));
            break;
        }
        case AArch64ISD::UITOF: {
            DEBUG(errs() << "ISD: UITOF\n");
            Type *ResType = ResEVT.getTypeForEVT(*Ctx);
            Value *Op = getNextOperand();
            if (!Op->getType()->isIntegerTy()) {
                Op = Builder->CreateBitCast(Op, IntegerType::get(getGlobalContext(), ResType->getScalarSizeInBits()));
            }
            registerResult(Builder->CreateUIToFP(Op, ResType));
            break;
        }
        case AArch64ISD::NVCAST: {
            DEBUG(errs() << "ISD: NVCAST\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::SMULL: {
            DEBUG(errs() << "ISD: SMULL\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::UMULL: {
            DEBUG(errs() << "ISD: UMULL\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD2post: {
            DEBUG(errs() << "ISD: LD2post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD3post: {
            DEBUG(errs() << "ISD: LD3post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD4post: {
            DEBUG(errs() << "ISD: LD4post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST2post: {
            DEBUG(errs() << "ISD: ST2post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST3post: {
            DEBUG(errs() << "ISD: ST3post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST4post: {
            DEBUG(errs() << "ISD: ST4post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD1x2post: {
            DEBUG(errs() << "ISD: LD1x2post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD1x3post: {
            DEBUG(errs() << "ISD: LD1x3post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD1x4post: {
            DEBUG(errs() << "ISD: LD1x4post\n");
            llvm_unreachable("Not implemented");
            break;
        } 
        case AArch64ISD::ST1x2post: {
            DEBUG(errs() << "ISD: ST1x2post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST1x3post: {
            DEBUG(errs() << "ISD: ST1x3post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST1x4post: {
            DEBUG(errs() << "ISD: ST1x4post\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD1DUPpost: {
            DEBUG(errs() << "ISD: LD1DUPpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD2DUPpost: {
            DEBUG(errs() << "ISD: LD2DUPpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD3DUPpost: {
            DEBUG(errs() << "ISD: LD3DUPpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD4DUPpost: {
            DEBUG(errs() << "ISD: LD4DUPpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD1LANEpost: {
            DEBUG(errs() << "ISD: LD1LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD2LANEpost: {
            DEBUG(errs() << "ISD: LD2LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD3LANEpost: {
            DEBUG(errs() << "ISD: LD3LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::LD4LANEpost: {
            DEBUG(errs() << "ISD: LD4LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST2LANEpost: {
            DEBUG(errs() << "ISD: ST2LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST3LANEpost: {
            DEBUG(errs() << "ISD: ST3LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::ST4LANEpost: {
            DEBUG(errs() << "ISD: ST4LANEpost\n");
            llvm_unreachable("Not implemented");
            break;
        }
        case AArch64ISD::AARCH_REG_EXT: {
            DEBUG(errs() << "ISD: AARCH_REG_EXT\n");
            Value *OffsetReg = getNextOperand();
            uint64_t ExtShift = cast<ConstantInt>(getNextOperand())->getValue().getZExtValue();
            uint64_t Ext = (ExtShift >> 32);
            uint64_t Shift = ExtShift ^ (Ext << 32);
            switch (Ext) {
                default:
                    errs() << Ext;
                    llvm_unreachable("Ext not implemented");
                case AArch64_AM::UXTW: {
                    OffsetReg = Builder->CreateTrunc(OffsetReg, Builder->getInt32Ty());
                    OffsetReg = Builder->CreateZExt(OffsetReg, Builder->getInt64Ty());
                    break;
                }
                case AArch64_AM::SXTX:
                case AArch64_AM::UXTX: {
                    break;
                }
                case AArch64_AM::SXTW: {
                    OffsetReg = Builder->CreateTrunc(OffsetReg, Builder->getInt32Ty());
                    OffsetReg = Builder->CreateSExt(OffsetReg, Builder->getInt64Ty());
                    break;
                }
            }

            OffsetReg = Builder->CreateShl(OffsetReg, Shift);
            registerResult(OffsetReg);
            break;
        }
        case AArch64ISD::AARCH_BC_I64: {
            DEBUG(errs() << "ISD: AARCH_REG_EXT\n");
            Value *V = getNextOperand();
            V = Builder->CreateBitCast(V, Builder->getInt64Ty());
            registerResult(V);
            break;
        }
    }

}

void AArch64InstrSema::translateTargetIntrinsic(unsigned IntrinsicID) {
    switch (IntrinsicID) {
        default:
            errs() << "[!]intrinsic not handled: " << IntrinsicID <<"\n";
            // llvm_unreachable("intrinsic not handled");
        case Intrinsic::aarch64_neon_abs:
        {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_addhn:
        {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);

            registerResult(result);
            
            break;
        }
        case Intrinsic::aarch64_neon_addp:
        {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);

            break;
        }
        //BugID: welink_1003F26A4
        case Intrinsic::aarch64_neon_cls: {
            Value *op = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op);

            std::vector<Type*> types;
            types.push_back(op->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            
            break;
        }
        case Intrinsic::aarch64_neon_fabd: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_facge: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_facgt: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_faddv: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_fcvtas:
        case Intrinsic::aarch64_neon_fcvtau:
        case Intrinsic::aarch64_neon_fcvtms:
        case Intrinsic::aarch64_neon_fcvtmu:
        case Intrinsic::aarch64_neon_fcvtns:
        case Intrinsic::aarch64_neon_fcvtnu:
        case Intrinsic::aarch64_neon_fcvtps:
        case Intrinsic::aarch64_neon_fcvtpu:
        case Intrinsic::aarch64_neon_fcvtxn:
        case Intrinsic::aarch64_neon_fcvtzs:
        case Intrinsic::aarch64_neon_fcvtzu: {
            Value *op = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            types.push_back(op->getType());
            
            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        //BugID: tzhuaTai_0x100C392A4
        case Intrinsic::aarch64_neon_fmax: 
        case Intrinsic::aarch64_neon_fmaxp:
        case Intrinsic::aarch64_neon_fmaxnm:
        case Intrinsic::aarch64_neon_fmaxnmp: {
            errs() << "[+]fmax handled: " << IntrinsicID <<"\n";
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);

            break;
        }
        case Intrinsic::aarch64_neon_fmaxv:
        case Intrinsic::aarch64_neon_fmaxnmv: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_fmin:
        case Intrinsic::aarch64_neon_fminp:
        case Intrinsic::aarch64_neon_fminnm:
        case Intrinsic::aarch64_neon_fminnmp: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);

            break;
        }
        case Intrinsic::aarch64_neon_fminv:
        case Intrinsic::aarch64_neon_fminnmv: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_fmulx: {
            //TODO
            break;
        }
        //BugID: tzhuaTai_100C2C8B4
        case Intrinsic::aarch64_neon_frecpe: 
        {
            Value *op1 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(op1->getType());


            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        //BugID: tzhuaTai_100C2C8B8
        case Intrinsic::aarch64_neon_frecps: 
        {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            // types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            types.push_back(op1->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_frecpx:
        case Intrinsic::aarch64_neon_frintn: {
            Value *op1 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_frsqrte: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_frsqrts: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_ld1x2: {
            //TODO
        }
        case Intrinsic::aarch64_neon_pmul:
        case Intrinsic::aarch64_neon_pmull:
        case Intrinsic::aarch64_neon_pmull64: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_raddhn: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_rbit: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_rshrn: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);

            break;
        }
        case Intrinsic::aarch64_neon_rsubhn: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_sabd: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_saddlp:
        case Intrinsic::aarch64_neon_saddlv:
        case Intrinsic::aarch64_neon_saddv: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_scalar_sqxtn:
        case Intrinsic::aarch64_neon_scalar_sqxtun:
        case Intrinsic::aarch64_neon_scalar_uqxtn: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_shadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_shll: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_shsub:{
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_smax:
        case Intrinsic::aarch64_neon_smin: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);

            break;
        }
        //BugID: welink_1003F3FD4
        case Intrinsic::aarch64_neon_smaxv:
        case Intrinsic::aarch64_neon_sminv: {
            Value *op1 = getNextOperand();

            std::vector<Value*> args;
            args.push_back(op1);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            types.push_back(op1->getType());


            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);

            break;
        }
        case Intrinsic::aarch64_neon_smaxp:
        case Intrinsic::aarch64_neon_sminp: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Value*> args;
            args.push_back(op1);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));


            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);

            break;
        }
        case Intrinsic::aarch64_neon_smull:
        {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqabs: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            // op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqadd: 
        case Intrinsic::aarch64_neon_sqdmulh: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        //BugID: koubei_10073EAB0
        case Intrinsic::aarch64_neon_sqdmull: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqdmulls_scalar: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_sqneg: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_sqrdmulh: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqrshl:
        case Intrinsic::aarch64_neon_sqrshrn:
        case Intrinsic::aarch64_neon_sqrshrun: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqshl: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqshlu:
        case Intrinsic::aarch64_neon_sqshrn:
        case Intrinsic::aarch64_neon_sqshrun: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqsub: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sqxtn:
        case Intrinsic::aarch64_neon_sqxtun: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_srhadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_srshl: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_sshl:
        case Intrinsic::aarch64_neon_sshll: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        //  {
        //     Value *op1 = getNextOperand();
        //     std::vector<Value*> args;
        //     args.push_back(op1);

        //     std::vector<Type*> types;
        //     types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

        //     Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
        //     registerResult(result);
        //     break;
        // }
        case Intrinsic::aarch64_neon_st1x2: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_st2: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_subhn: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);

            registerResult(result);
            
            break;
        }
        case Intrinsic::aarch64_neon_suqadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_tbl1: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_uabd: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_uaddlv:
        case Intrinsic::aarch64_neon_uaddlp: {
            Value *op = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_uaddv: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());


            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_uhadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        //BugID: koubei_1003D3150
        case Intrinsic::aarch64_neon_uhsub: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_umax:
        case Intrinsic::aarch64_neon_umin: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_umaxv:
        case Intrinsic::aarch64_neon_uminv: {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);
            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_umaxp:
        case Intrinsic::aarch64_neon_uminp: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_umull: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_uqadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_uqshl:
        case Intrinsic::aarch64_neon_uqrshl:
        //BugID: tzhuaTai_1012FC820
        case Intrinsic::aarch64_neon_uqrshrn:{
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        //BugID: tzhuaTai_1012FBF28
        case Intrinsic::aarch64_neon_uqshrn:
        {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);

            break;
        }
        case Intrinsic::aarch64_neon_uqsub:
        {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_uqxtn:
        {
            Value *op = getNextOperand();

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(*Ctx));
            types.push_back(op->getType());

            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);

            op = Builder->CreateZExt(op, intrinsic->getArgumentList().front().getType());
            std::vector<Value*> args;
            args.push_back(op);

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_urecpe: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_urhadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_urshl: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_ursqrte: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_ushl: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_ushll: {
            Value *op1 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_usqadd: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_vcopy_lane: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_vcvtfp2fxs:
        case Intrinsic::aarch64_neon_vcvtfp2fxu: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_vcvtfp2hf:
        case Intrinsic::aarch64_neon_vcvthf2fp: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_neon_vcvtfxs2fp:
        case Intrinsic::aarch64_neon_vcvtfxu2fp: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_vsli: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *op3 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);
            args.push_back(op3);

            std::vector<Type*> types;
            types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            //   types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_neon_vsri: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            Value *op3 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);
            // args.push_back(op3);

            std::vector<Type*> types;
            // types.push_back(ResEVT.getTypeForEVT(getGlobalContext()));
            types.push_back(op1->getType());
            Function *intrinsic = Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types);
            op3 = Builder->CreateZExtOrTrunc(op3, intrinsic->getArgumentList().back().getType());
            args.push_back(op3);

            Value *result = Builder->CreateCall(intrinsic, args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_rbit:
        {
            Value *op = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op);

            std::vector<Type*> types;
            types.push_back(op->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_sdiv:
        case Intrinsic::aarch64_udiv: {
            Value *op1 = getNextOperand();
            Value *op2 = getNextOperand();
            std::vector<Value*> args;
            args.push_back(op1);
            args.push_back(op2);

            std::vector<Type*> types;
            types.push_back(op1->getType());

            Value *result = Builder->CreateCall(Intrinsic::getDeclaration(TheModule, (llvm::Intrinsic::ID)IntrinsicID, types), args);
            registerResult(result);
            break;
        }
        case Intrinsic::aarch64_sisd_fabd: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_sisd_fcvtxn: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_stlxp: {
            //TOTO
            break;
        }
        case Intrinsic::aarch64_stlxr: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_stxp: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_stxr: {
            //TODO
            break;
        }
        case Intrinsic::aarch64_thread_pointer: {
            //TODO
            break;
        }
        //TODO some arm intr
    }
}