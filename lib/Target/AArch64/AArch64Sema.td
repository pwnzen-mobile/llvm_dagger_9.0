include "llvm/DC/Semantics.td"

class Sema_F<Instruction inst, dag pattern>
  : Semantics<inst, [pattern, (implicit NZCV)]>;

class Sema_F_Str<string inst, dag pattern>
: Sema_F<!cast<Instruction>(inst), pattern>;

// suppress errors, not correct yet!

def : Semantics<ADCSWr, [(set GPR32:$Rd, NZCV, (AArch64adc_flag GPR32:$Rn, GPR32:$Rm, NZCV))]>;
def : Semantics<ADCSXr, [(set GPR64:$Rd, NZCV, (AArch64adc_flag GPR64:$Rn, GPR64:$Rm, NZCV))]>;

def : Semantics<ADDSWri, [(set GPR32:$Rd, NZCV, (AArch64add_flag GPR32sp:$Rn, addsub_shifted_imm32:$imm))]>;
def : Semantics<ADDSWrr, [(set GPR32:$Rd, NZCV, (AArch64add_flag GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<ADDSWrs, [(set GPR32:$Rd, NZCV, (AArch64add_flag GPR32:$Rn, arith_shifted_reg32:$Rm))]>;
//def : Semantics<ADDSWrx, [(set GPR32:$R1, NZCV, (AArch64add_flag GPR32sp:$R2, ADDSanonymous_755:$R3))]>;
def : Semantics<ADDSXri, [(set GPR64:$Rd, NZCV, (AArch64add_flag GPR64sp:$Rn, addsub_shifted_imm64:$imm))]>;
def : Semantics<ADDSXrr, [(set GPR64:$Rd, NZCV, (AArch64add_flag GPR64:$Rn, GPR64:$Rm))]>;
def : Semantics<ADDSXrs, [(set GPR64:$Rd, NZCV, (AArch64add_flag GPR64:$Rn, arith_shifted_reg64:$Rm))]>;
//def : Semantics<ADDSXrx, [(set GPR64:$R1, NZCV, (AArch64add_flag GPR64sp:$R2, ADDSanonymous_756:$R3))]>;

def : Semantics<ANDSWri, [(set GPR32:$Rd, NZCV, (AArch64and_flag GPR32:$Rn, logical_imm32:$imm))]>;
def : Semantics<ANDSWrr, [(set GPR32:$Rd, NZCV, (AArch64and_flag GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<ANDSWrs, [(set GPR32:$Rd, NZCV, (AArch64and_flag GPR32:$Rn, logical_shifted_reg32:$Rm))]>;
def : Semantics<ANDSXri, [(set GPR64:$Rd, NZCV, (AArch64and_flag GPR64:$Rn, logical_imm64:$imm))]>;
def : Semantics<ANDSXrr, [(set GPR64:$Rd, NZCV, (AArch64and_flag GPR64:$Rn, GPR64:$Rm))]>;
def : Semantics<ANDSXrs, [(set GPR64:$Rd, NZCV, (AArch64and_flag GPR64:$Rn, logical_shifted_reg64:$Rm))]>;

def : Semantics<ANDWrr, [(set GPR32:$Rd, (and GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<ANDXrr, [(set GPR64:$Rd, (and GPR64:$Rn, GPR64:$Rm))]>;

def : Semantics<ADDWrr, [(set GPR32:$Rd, (add GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<ADDXrr, [(set GPR64:$Rd, (add GPR64:$Rn, GPR64:$Rm))]>;

def : Semantics<SBCSWr, [(set GPR32:$Rd, NZCV, (AArch64sbc_flag GPR32:$Rn, GPR32:$Rm, NZCV)), (implicit NZCV)]>;
def : Semantics<SBCSXr, [(set GPR64:$Rd, NZCV, (AArch64sbc_flag GPR64:$Rn, GPR64:$Rm, NZCV)), (implicit NZCV)]>;

def : Semantics<SUBSWri, [(set GPR32:$Rd, NZCV, (AArch64sub_flag GPR32sp:$Rn, addsub_shifted_imm32:$imm))]>;
def : Semantics<SUBSWrr, [(set GPR32:$Rd, NZCV, (AArch64sub_flag GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<SUBSWrs, [(set GPR32:$Rd, NZCV, (AArch64sub_flag GPR32:$Rn, arith_shifted_reg32:$Rm))]>;
//def : Semantics<SUBSWrx, [(set GPR32:$R1, NZCV, (AArch64sub_flag GPR32sp:$R2, SUBSanonymous_755:$R3))]>;
def : Semantics<SUBSXri, [(set GPR64:$Rd, NZCV, (AArch64sub_flag GPR64sp:$Rn, addsub_shifted_imm64:$imm))]>;
def : Semantics<SUBSXrr, [(set GPR64:$Rd, NZCV, (AArch64sub_flag GPR64:$Rn, GPR64:$Rm))]>;
def : Semantics<SUBSXrs, [(set GPR64:$Rd, NZCV, (AArch64sub_flag GPR64:$Rn, arith_shifted_reg64:$Rm))]>;
//def : Semantics<SUBSXrx, [(set GPR64:$R1, NZCV, (AArch64sub_flag GPR64sp:$R2, SUBSanonymous_756:$R3))]>;

def : Semantics<STURXi, [(store GPR64:$Rt, (add GPR64sp:$Rn, simm9:$offset))]>;

def : Semantics<BICSWrs, [(set GPR32:$Rd, (add GPR32:$Rn, logical_shifted_reg32:$Rm))]>;
def : Semantics<BICSXrs, [(set GPR64:$Rd, (add GPR64:$Rn, logical_shifted_reg64:$Rm))]>;


def : Semantics<LDRBBui, [(set GPR32:$Rt, (zext (i8 (load (add GPR64sp:$Rn, uimm12s1:$offset)))))]>;
def : Semantics<LDRBui, [(set FPR8:$Rt, (bitconvert (i8 (load (add GPR64sp:$Rn, uimm12s1:$offset)))))]>;
def : Semantics<LDRDui, [(set (f64 FPR64:$Rt), (load (add GPR64sp:$Rn, uimm12s8:$offset)))]>;
def : Semantics<LDRHHui, [(set GPR32:$Rt, (zext (i16 (load (add GPR64sp:$Rn, uimm12s2:$offset)))))]>;
def : Semantics<LDRHui, [(set (f16 FPR16:$Rt), (load (add GPR64sp:$Rn, uimm12s2:$offset)))]>;
def : Semantics<LDRQui, [(set (f128 FPR128:$Rt), (load (add GPR64sp:$Rn, uimm12s16:$offset)))]>;
def : Semantics<LDRSBWui, [(set GPR32:$Rt, (sext (i8 (load (add GPR64sp:$Rn, uimm12s1:$offset)))))]>;
def : Semantics<LDRSBXui, [(set GPR64:$Rt, (sext (i8 (load (add GPR64sp:$Rn, uimm12s1:$offset)))))]>;
def : Semantics<LDRSHWui, [(set GPR32:$Rt, (sext (i16 (load (add GPR64sp:$Rn, uimm12s2:$offset)))))]>;
def : Semantics<LDRSHXui, [(set GPR64:$Rt, (sext (i16 (load (add GPR64sp:$Rn, uimm12s2:$offset)))))]>;
def : Semantics<LDRSWui, [(set GPR64:$Rt, (sext (i32 (load (add GPR64sp:$Rn, uimm12s4:$offset)))))]>;
def : Semantics<LDRSui, [(set (f32 FPR32:$Rt), (load (add GPR64sp:$Rn, uimm12s4:$offset)))]>;
def : Semantics<LDRWui, [(set GPR32:$Rt, (load (add GPR64sp:$Rn, uimm12s4:$offset)))]>;
def : Semantics<LDRXui, [(set GPR64:$Rt, (load (add GPR64sp:$Rn, uimm12s8:$offset)))]>;
def : Semantics<LDURBBi, [(set GPR32:$Rt, (zext (i8 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURBi, [(set FPR8:$Rt, (bitconvert (i8 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURDi, [(set (f64 FPR64:$Rt), (load (add GPR64sp:$Rn, simm9:$offset)))]>;
def : Semantics<LDURHHi, [(set GPR32:$Rt, (zext (i16 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURHi, [(set FPR16:$Rt, (load (add GPR64sp:$Rn, simm9:$offset)))]>;
def : Semantics<LDURQi, [(set (f128 FPR128:$Rt), (load (add GPR64sp:$Rn, simm9:$offset)))]>;
def : Semantics<LDURSBWi, [(set GPR32:$Rt, (sext (i8 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURSBXi, [(set GPR64:$Rt, (sext (i8 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURSHWi, [(set GPR32:$Rt, (sext (i16 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURSHXi, [(set GPR64:$Rt, (sext (i16 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURSWi, [(set GPR64:$Rt, (sext (i32 (load (add GPR64sp:$Rn, simm9:$offset)))))]>;
def : Semantics<LDURSi, [(set (f32 FPR32:$Rt), (load (add GPR64sp:$Rn, simm9:$offset)))]>;
def : Semantics<LDURWi, [(set GPR32:$Rt, (load (add GPR64sp:$Rn, simm9:$offset)))]>;
def : Semantics<LDURXi, [(set GPR64:$Rt, (load (add GPR64sp:$Rn, simm9:$offset)))]>;

def : Semantics<STRBBui, [(store (i8 (trunc GPR32:$Rt)), (add GPR64sp:$Rn, uimm12s1:$offset))]>;
def : Semantics<STRBui, [(store FPR8:$Rt, (add GPR64sp:$Rn, uimm12s1:$offset))]>;
def : Semantics<STRDui, [(store (f64 FPR64:$Rt), (add GPR64sp:$Rn, uimm12s8:$offset))]>;
def : Semantics<STRQui, [(store (f128 FPR128:$Rt), (add GPR64sp:$Rn, uimm12s16:$offset))]>;
def : Semantics<STRHHui, [(store (i16 (trunc GPR32:$Rt)), (add GPR64sp:$Rn, uimm12s2:$offset))]>;
def : Semantics<STRHui, [(store (f16 FPR16:$Rt), (add GPR64sp:$Rn, uimm12s2:$offset))]>;
def : Semantics<STRSui, [(store (f32 FPR32:$Rt), (add GPR64sp:$Rn, uimm12s4:$offset))]>;
def : Semantics<STRWui, [(store GPR32:$Rt, (add GPR64sp:$Rn, uimm12s4:$offset))]>;
def : Semantics<STRXui, [(store GPR64:$Rt, (add GPR64sp:$Rn, uimm12s8:$offset))]>;
def : Semantics<STURBBi, [(store (i8 (trunc GPR32:$Rt)), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURBi, [(store (i8 (bitconvert FPR8:$Rt)), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURDi, [(store (f64 FPR64:$Rt), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURHHi, [(store (i16 (trunc GPR32:$Rt)), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURHi, [(store (f16 FPR16:$Rt), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURQi, [(store (f128 FPR128:$Rt), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURSi, [(store (f32 FPR32:$Rt), (add GPR64sp:$Rn, simm9:$offset))]>;
def : Semantics<STURWi, [(store GPR32:$Rt, (add GPR64sp:$Rn, simm9:$offset))]>;


//Start of "real" definitions

def AArchDec_Ext64  : SDNode<"AArch64ISD::AARCH_REG_EXT",  SDTypeProfile<1, 2, []>>;

class Sema_LDR_ro<Instruction inst, ValueType DstType, DAGOperand DstReg, string LoadOp, int LoadWidth, int OffsetRegWidth> : Semantics<inst,
  [(set (DstType DstReg:$Rt), (!cast<SDPatternOperator>(LoadOp) (add GPR64sp:$Rn, (AArchDec_Ext64 !cast<DAGOperand>("GPR"#OffsetRegWidth):$Rm, !cast<Operand>("ro_"#!if(!eq(OffsetRegWidth, 64), "X", "W")#"extend"#LoadWidth):$extend))))]>;

class Sema_zext_LDR_ro<Instruction inst, ValueType DstType, DAGOperand DstReg , int LoadWidth, int OffsetRegWidth> :
  Semantics<inst, [(set (DstType DstReg:$Rt), (zext (!cast<ValueType>("i"#LoadWidth) (load (add GPR64sp:$Rn, (AArchDec_Ext64 !cast<DAGOperand>("GPR"#OffsetRegWidth):$Rm, !cast<Operand>("ro_"#!if(!eq(OffsetRegWidth, 64), "X", "W")#"extend"#LoadWidth):$extend))))))]>;

class Sema_sext_LDR_ro<Instruction inst, ValueType DstType, DAGOperand DstReg , int LoadWidth, int OffsetRegWidth> :
  Semantics<inst, [(set (DstType DstReg:$Rt), (sext (!cast<ValueType>("i"#LoadWidth) (load (add GPR64sp:$Rn, (AArchDec_Ext64 !cast<DAGOperand>("GPR"#OffsetRegWidth):$Rm, !cast<Operand>("ro_"#!if(!eq(OffsetRegWidth, 64), "X", "W")#"extend"#LoadWidth):$extend))))))]>;

def : Sema_zext_LDR_ro<LDRBBroX, i32, GPR32, 8, 64>;
def : Sema_zext_LDR_ro<LDRBBroW, i32, GPR32, 8, 32>;

def : Semantics<LDRBroW, [(set FPR8:$Rt, (bitconvert (i8 (load (add GPR64sp:$Rn, (AArchDec_Ext64 GPR32:$Rm, ro_Wextend8:$extend))))))]>;
def : Semantics<LDRBroX, [(set FPR8:$Rt, (bitconvert (i8 (load (add GPR64sp:$Rn, (AArchDec_Ext64 GPR64:$Rm, ro_Xextend8:$extend))))))]>;

def : Sema_LDR_ro<LDRDroW, f64, FPR64, "load", 64, 32>;
def : Sema_LDR_ro<LDRDroX, f64, FPR64, "load", 64, 64>;

def : Sema_zext_LDR_ro<LDRHHroX, i32, GPR32, 16, 64>;
def : Sema_zext_LDR_ro<LDRHHroW, i32, GPR32, 16, 32>;

def : Sema_LDR_ro<LDRHroW, f16, FPR16, "load", 16, 32>;
def : Sema_LDR_ro<LDRHroX, f16, FPR16, "load", 16, 64>;

def : Sema_LDR_ro<LDRQroW, f128, FPR128, "load", 128, 32>;
def : Sema_LDR_ro<LDRQroX, f128, FPR128, "load", 128, 64>;

def : Sema_sext_LDR_ro<LDRSBWroX, i32, GPR32, 8, 64>;
def : Sema_sext_LDR_ro<LDRSBWroW, i32, GPR32, 8, 32>;

def : Sema_sext_LDR_ro<LDRSBXroX, i64, GPR64, 8, 64>;
def : Sema_sext_LDR_ro<LDRSBXroW, i64, GPR64, 8, 32>;

def : Sema_sext_LDR_ro<LDRSHWroX, i32, GPR32, 16, 64>;
def : Sema_sext_LDR_ro<LDRSHWroW, i32, GPR32, 16, 32>;

def : Sema_sext_LDR_ro<LDRSHXroX, i64, GPR64, 16, 64>;
def : Sema_sext_LDR_ro<LDRSHXroW, i64, GPR64, 16, 32>;

def : Sema_sext_LDR_ro<LDRSWroX, i64, GPR64, 32, 64>;
def : Sema_sext_LDR_ro<LDRSWroW, i64, GPR64, 32, 32>;


def : Sema_LDR_ro<LDRSroW, f32, FPR32, "load", 32, 32>;
def : Sema_LDR_ro<LDRSroX, f32, FPR32, "load", 32, 64>;

def : Sema_LDR_ro<LDRWroW, i32, GPR32, "load", 32, 32>;
def : Sema_LDR_ro<LDRWroX, i32, GPR32, "load", 32, 64>;

def : Sema_LDR_ro<LDRXroW, i64, GPR64, "load", 64, 32>;
def : Sema_LDR_ro<LDRXroX, i64, GPR64, "load", 64, 64>;



class Sema_STR_ro<Instruction inst, string StoreOp, ValueType SrcType, DAGOperand SrcReg, int StoreWidth, int OffsetRegWidth> :
  Semantics<inst, [(!cast<SDPatternOperator>(StoreOp) (SrcType SrcReg:$Rt), (add GPR64sp:$Rn, (AArchDec_Ext64 !cast<DAGOperand>("GPR"#OffsetRegWidth):$Rm, !cast<Operand>("ro_"#!if(!eq(OffsetRegWidth, 64), "X", "W")#"extend"#StoreWidth):$extend)))]>;

def : Sema_STR_ro<STRBBroW, "truncstorei8", i32, GPR32, 8, 32>;
def : Sema_STR_ro<STRBBroX, "truncstorei8", i32, GPR32, 8, 64>;

def : Sema_STR_ro<STRBroW, "store", untyped, FPR8, 8, 32>;
def : Sema_STR_ro<STRBroX, "store", untyped, FPR8, 8, 64>;

def : Sema_STR_ro<STRDroW, "store", f64, FPR64, 64, 32>;
def : Sema_STR_ro<STRDroX, "store", f64, FPR64, 64, 64>;

def : Sema_STR_ro<STRHHroW, "truncstorei16", i32, GPR32, 16, 32>;
def : Sema_STR_ro<STRHHroX, "truncstorei16", i32, GPR32, 16, 64>;

def : Sema_STR_ro<STRHroW, "store", f16, FPR16, 16, 32>;
def : Sema_STR_ro<STRHroX, "store", f16, FPR16, 16, 64>;

def : Sema_STR_ro<STRQroW, "store", f128, FPR128, 128, 32>;
def : Sema_STR_ro<STRQroX, "store", f128, FPR128, 128, 64>;

def : Sema_STR_ro<STRSroW, "store", f32, FPR32, 32, 32>;
def : Sema_STR_ro<STRSroX, "store", f32, FPR32, 32, 64>;

def : Sema_STR_ro<STRWroW, "store", i32, GPR32, 32, 32>;
def : Sema_STR_ro<STRWroX, "store", i32, GPR32, 32, 64>;

def : Sema_STR_ro<STRXroW, "store", i64, GPR64, 64, 32>;
def : Sema_STR_ro<STRXroX, "store", i64, GPR64, 64, 64>;


//Dummies
def : Semantics<PRFMroW, [(AArch64Prefetch imm:$Rt, (add GPR64sp:$Rn, (AArchDec_Ext64 GPR32:$Rm, ro_Wextend64:$extend)))]>;
def : Semantics<PRFMroX, [(AArch64Prefetch imm:$Rt, (add GPR64sp:$Rn, (AArchDec_Ext64 GPR64:$Rm, ro_Xextend64:$extend)))]>;

def : Semantics<PRFMui, [(AArch64Prefetch imm:$Rt, (add GPR64sp:$Rn, uimm12s8:$offset))]>;
def : Semantics<PRFUMi, [(AArch64Prefetch imm:$Rt, (add GPR64sp:$Rn, simm9:$offset))]>;



def : Semantics<MOVZXi, [(set GPR64:$Rd ,(shl (i64 (zext movimm32_imm:$imm)), movimm64_shift:$shift))]>;
def : Semantics<MOVZWi, [(set GPR32:$Rd ,(shl movimm32_imm:$imm, movimm32_shift:$shift))]>;

def : Semantics<MOVNXi, [(set GPR64:$Rd ,(xor -1, (shl (i64 (zext movimm32_imm:$imm)), movimm64_shift:$shift)))]>;
def : Semantics<MOVNWi, [(set GPR32:$Rd ,(xor -1, (shl movimm32_imm:$imm, movimm32_shift:$shift)))]>;

//Redefinitions of Operands

def : Semantics<ADDXrx, [(set GPR64sp:$R1, (add GPR64sp:$R2, arith_extended_reg32to64<i64>:$R3))]>;
def : Semantics<SUBXrx, [(set GPR64sp:$R1, (sub GPR64sp:$R2, arith_extended_reg32to64<i64>:$R3))]>;
def : Semantics<SUBWrx, [(set GPR32sp:$R1, (sub GPR32sp:$R2, arith_extended_reg32<i32>:$R3))]>;
def : Semantics<SUBXri, [(set GPR64sp:$Rd, (sub GPR64sp:$Rn, addsub_shifted_imm64:$imm))]>;
def : Semantics<SUBWrs, [(set GPR32:$Rd, (sub GPR32:$Rn, arith_shifted_reg32:$Rm))]>;
def : Semantics<SUBXrs, [(set GPR64:$Rd, (sub GPR64:$Rn, arith_shifted_reg64:$Rm))]>;


class MADD<Instruction inst, int Width> : Semantics<inst,
  [(set !cast<DAGOperand>("GPR"#Width):$Rd, (add !cast<DAGOperand>("GPR"#Width):$Ra, (mul !cast<DAGOperand>("GPR"#Width):$Rn, !cast<DAGOperand>("GPR"#Width):$Rm)))]>;
def : MADD<MADDWrrr, 32>;
def : MADD<MADDXrrr, 64>;


class MSUB<Instruction inst, int Width> : Semantics<inst,
  [(set !cast<DAGOperand>("GPR"#Width):$Rd, (sub !cast<DAGOperand>("GPR"#Width):$Ra, (mul !cast<DAGOperand>("GPR"#Width):$Rn, !cast<DAGOperand>("GPR"#Width):$Rm)))]>;
def : MSUB<MSUBWrrr, 32>;
def : MSUB<MSUBXrrr, 64>;


//STP LDP

class STP<Instruction inst, ValueType SrcType, DAGOperand SrcOperand, int Width> : Semantics<inst,
  [(store (SrcType SrcOperand:$Rt), (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset)))), (store (SrcType SrcOperand:$Rt2), (add Width, (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset)))))]>;

class STPpost<Instruction inst, ValueType SrcType, DAGOperand SrcOperand, int Width> : Semantics<inst,
  [(store (SrcType SrcOperand:$Rt), GPR64sp:$Rn), (store (SrcType SrcOperand:$Rt2), (add Width, GPR64sp:$Rn)), (set GPR64sp:$wback, (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset))))]>;

class STPpre<Instruction inst, ValueType SrcType, DAGOperand SrcOperand, int Width> : Semantics<inst,
  [(set GPR64sp:$wback, (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset)))), (store (SrcType SrcOperand:$Rt), GPR64sp:$wback), (store (SrcType SrcOperand:$Rt2), (add Width, GPR64sp:$wback))]>;

def : STP<STPWi, i32, GPR32, 4>;
def : STP<STPXi, i64, GPR64, 8>;
def : STP<STPSi, f32, FPR32, 4>;
def : STP<STPDi, f64, FPR64, 8>;
def : STP<STPQi, f128, FPR128, 16>;

def : STPpre<STPWpre, i32, GPR32, 4>;
def : STPpre<STPXpre, i64, GPR64, 8>;
def : STPpre<STPSpre, f32, FPR32, 4>;
def : STPpre<STPDpre, f64, FPR64, 8>;
def : STPpre<STPQpre, f128, FPR128, 16>;

def : STPpost<STPWpost, i32, GPR32, 4>;
def : STPpost<STPXpost, i64, GPR64, 8>;
def : STPpost<STPSpost, f32, FPR32, 4>;
def : STPpost<STPDpost, f64, FPR64, 8>;
def : STPpost<STPQpost, f128, FPR128, 16>;

class LDP<Instruction inst, ValueType DstType, DAGOperand DstOperand, int Width> : Semantics<inst,
  [(set (DstType DstOperand:$Rt), (load (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset))))), (set (DstType DstOperand:$Rt2), (load (add Width, (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset))))))]>;

class LDPpre<Instruction inst, ValueType DstType, DAGOperand DstOperand, int Width> : Semantics<inst,
  [(set GPR64sp:$wback, (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset)))), (set (DstType DstOperand:$Rt), (load GPR64sp:$wback)), (set (DstType DstOperand:$Rt2), (load (add Width, GPR64sp:$wback)))]>;

class LDPpost<Instruction inst, ValueType DstType, DAGOperand DstOperand, int Width> : Semantics<inst,
  [(set (DstType DstOperand:$Rt), (load GPR64sp:$Rn)), (set (DstType DstOperand:$Rt2), (load (add Width, GPR64sp:$Rn))), (set GPR64sp:$wback, (add GPR64sp:$Rn, (i64 (sext !cast<DAGOperand>("simm7s"#Width):$offset))))]>;

def : LDP<LDPWi, i32, GPR32, 4>;
def : LDP<LDPXi, i64, GPR64, 8>;
def : LDP<LDPSi, f32, FPR32, 4>;
def : LDP<LDPDi, f64, FPR64, 8>;
def : LDP<LDPQi, f128, FPR128, 16>;

def : LDPpost<LDPWpost, i32, GPR32, 4>;
def : LDPpost<LDPXpost, i64, GPR64, 8>;
def : LDPpost<LDPSpost, f32, FPR32, 4>;
def : LDPpost<LDPDpost, f64, FPR64, 8>;
def : LDPpost<LDPQpost, f128, FPR128, 16>;

def : LDPpre<LDPWpre, i32, GPR32, 4>;
def : LDPpre<LDPXpre, i64, GPR64, 8>;
def : LDPpre<LDPSpre, f32, FPR32, 4>;
def : LDPpre<LDPDpre, f64, FPR64, 8>;
def : LDPpre<LDPQpre, f128, FPR128, 16>;

def : Semantics<LDPSWi , [(set GPR64:$Rt, (sext (i32 (load (add GPR64sp:$Rn, (i64 (sext simm7s4:$offset))))))), (set GPR64:$Rt2, (sext (i32 (load (add 4, (add GPR64sp:$Rn, (i64 (sext simm7s4:$offset))))))))]>;
def : Semantics<LDPSWpost , [(set GPR64:$Rt, (sext (i32 (load GPR64sp:$Rn)))), (set GPR64:$Rt2, (sext (i32 (load (add 4, GPR64sp:$Rn))))), (set GPR64sp:$wback, (add GPR64sp:$Rn, (i64 (sext simm7s4:$offset))))]>;
def : Semantics<LDPSWpre , [(set GPR64sp:$wback, (add GPR64sp:$Rn, (i64 (sext simm7s4:$offset)))), (set GPR64:$Rt, (sext (i32 (load GPR64sp:$wback)))), (set GPR64:$Rt2, (sext (i32 (load (add 4, GPR64sp:$wback)))))]>;


//LDR STR

//TODO: isn't this the 'pre' pattern???
class LDRpost<Instruction inst, int LoadWidth> : Semantics<inst,
  [(set GPR32:$Rt, (zext (!cast<ValueType>("i"#LoadWidth) (load GPR64sp:$Rn)))), (set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset))]>;

def : LDRpost<LDRBBpost, 8>;
def : LDRpost<LDRHHpost, 16>;


//Load and extend

class LDR_zext<Instruction inst, int LoadWidth> : Semantics<inst,
 [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set GPR32:$Rt, (zext (!cast<ValueType>("i"#LoadWidth) (load GPR64sp:$wback))))]>;

class LDR_sext_pre<Instruction inst, int LoadWidth> : Semantics<inst,
 [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set GPR32:$Rt, (sext (!cast<ValueType>("i"#LoadWidth) (load GPR64sp:$wback))))]>;

class LDR_sext64_pre<Instruction inst, int LoadWidth> : Semantics<inst,
  [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set GPR64:$Rt, (sext (!cast<ValueType>("i"#LoadWidth) (load GPR64sp:$wback))))]>;

class LDR_pre<Instruction inst, ValueType DstType, DAGOperand DstOperand> : Semantics<inst,
  [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set (DstType DstOperand:$Rt), (load GPR64sp:$wback))]>;

class LDR_post<Instruction inst, ValueType DstType, DAGOperand DstOperand> : Semantics<inst,
  [(set (DstType DstOperand:$Rt), (load GPR64sp:$Rn)), (set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset))]>;

class LDR_sext_post<Instruction inst, DAGOperand DstOperand, int LoadWidth> : Semantics<inst,
  [(set DstOperand:$Rt, (sext (!cast<ValueType>("i"#LoadWidth) (load GPR64sp:$Rn)))), (set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset))]>;

class STR_post<Instruction inst, ValueType SrcType, DAGOperand SrcOperand> : Semantics<inst,
  [(store (SrcType SrcOperand:$Rt), GPR64sp:$Rn), (set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset))]>;

class STR_trunc_post<Instruction inst, int StoreWidth> : Semantics<inst,
  [(store (!cast<ValueType>("i"#StoreWidth) (trunc GPR32:$Rt)), GPR64sp:$Rn), (set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset))]>;

class STR_trunc_pre<Instruction inst, int StoreWidth> : Semantics<inst,
  [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (store (!cast<ValueType>("i"#StoreWidth) (trunc GPR32:$Rt)), GPR64sp:$wback)]>;

class STR_pre<Instruction inst, DAGOperand SrcOperand> : Semantics<inst,
  [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (store SrcOperand:$Rt, GPR64sp:$wback)]>;

def : STR_trunc_post<STRBBpost, 8>;
def : STR_trunc_post<STRHHpost, 16>;

def : STR_post<STRXpost, i64, GPR64>;
def : STR_post<STRWpost, i32, GPR32>;
def : STR_post<STRSpost, f32, FPR32>;
def : STR_post<STRDpost, f64, FPR64>;
def : STR_post<STRQpost, f128, FPR128>;

def : Semantics<STRDpre, [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (store (f64 FPR64:$Rt), GPR64sp:$wback)]>;
def : Semantics<STRQpre, [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (store (f128 FPR128:$Rt), GPR64sp:$wback)]>;

def : STR_pre<STRXpre, GPR64>;
def : STR_pre<STRWpre, GPR32>;

def : STR_trunc_pre<STRBBpre, 8>;
def : STR_trunc_pre<STRHHpre, 16>;

def : LDR_pre<LDRXpre, i64, GPR64>;
def : LDR_pre<LDRWpre, i32, GPR32>;
def : LDR_pre<LDRSpre, f32, FPR32>;
def : LDR_pre<LDRQpre, f128, FPR128>;
def : LDR_pre<LDRDpre, f64, FPR64>;
def : LDR_pre<LDRHpre, f16, FPR16>;
def : LDR_sext_pre<LDRSBWpre, 8>;
def : LDR_sext_pre<LDRSHWpre, 16>;
def : LDR_sext64_pre<LDRSWpre, 32>;
def : LDR_post<LDRXpost, i64, GPR64>;
def : LDR_post<LDRWpost, i32, GPR32>;
def : LDR_post<LDRDpost, f64, FPR64>;
def : LDR_post<LDRSpost, f32, FPR32>;
def : LDR_post<LDRHpost, f16, FPR16>;
def : LDR_post<LDRQpost, f128, FPR128>;

def : LDR_zext<LDRBBpre, 8>;
def : LDR_sext_post<LDRSBWpost, GPR32, 8>;
def : LDR_sext_post<LDRSWpost, GPR64, 32>;
def : LDR_sext_post<LDRSHXpost, GPR64, 16>;
def : LDR_sext_post<LDRSHWpost, GPR32, 16>;
def : LDR_sext_post<LDRSBXpost, GPR64, 8>;

def : Semantics<LDRSBXpre, [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set GPR64:$Rt, (sext (i8 (load GPR64sp:$wback))))]>;

def : Semantics<ASRVWr, [(set GPR32:$Rd, (sra GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<ASRVXr, [(set GPR64:$Rd, (sra GPR64:$Rn, GPR64:$Rm))]>;

def : Semantics<LSLVWr, [(set GPR32:$Rd, (shl GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<LSLVXr, [(set GPR64:$Rd, (shl GPR64:$Rn, GPR64:$Rm))]>;

def : Semantics<LSRVWr, [(set GPR32:$Rd, (srl GPR32:$Rn, GPR32:$Rm))]>;
def : Semantics<LSRVXr, [(set GPR64:$Rd, (srl GPR64:$Rn, GPR64:$Rm))]>;

def : Semantics<RORVWr, [(set GPR32:$Rd, (or (shl GPR32:$Rn, (sub (i32 32), GPR32:$Rm)), (srl GPR32:$Rn, GPR32:$Rm)))]>;
def : Semantics<RORVXr, [(set GPR64:$Rd, (or (shl GPR64:$Rn, (sub (i64 64), GPR64:$Rm)), (srl GPR64:$Rn, GPR64:$Rm)))]>;


class LDAXR<Instruction inst, ValueType DstType, DAGOperand DstOperand, SDPatternOperator Op> :
  Semantics<inst, [(set (DstType DstOperand:$Rt), (Op GPR64sp0:$Rn))]>;

def : LDAXR<LDAXRW, i32, GPR32, load>;
def : LDAXR<LDAXRX, i64, GPR64, load>;
def : LDAXR<LDAXRB, i32, GPR32, zextloadi8>;
def : LDAXR<LDAXRH, i32, GPR32, zextloadi16>;

//TODO: maybe the status register should not be static...
class STLXR<Instruction inst, ValueType SrcType, DAGOperand SrcOperand, SDPatternOperator Op> :
  Semantics<inst, [(set GPR32:$Ws, 0), (Op (SrcType SrcOperand:$Rt), GPR64sp0:$Rn)]>;

def : STLXR<STLXRB, i32, GPR32, truncstorei8>;
def : STLXR<STLXRH, i32, GPR32, truncstorei16>;
def : STLXR<STLXRW, i32, GPR32, store>;
def : STLXR<STLXRX, i64, GPR64, store>;

class STXP<Instruction inst, ValueType SrcType, DAGOperand SrcOperand, int StoreWidth> : Semantics<inst,
  [(set GPR32:$Ws, 0), (store (SrcType SrcOperand:$Rt), GPR64sp0:$Rn), (store (SrcType SrcOperand:$Rt2), (add StoreWidth, GPR64sp0:$Rn))]>;

def : STXP<STXPW, i32, GPR32, 4>;
def : STXP<STXPX, i64, GPR64, 8>;

def : STXP<STLXPW, i32, GPR32, 4>;
def : STXP<STLXPX, i64, GPR64, 8>;


def : Semantics<LDRSWl, [(set GPR64:$Rt, (sext (i32 (load (i64 (bitconvert am_ldrlit:$label))))))]>;
def : Semantics<LDRWl, [(set GPR32:$Rt, (load (i64 (bitconvert am_ldrlit:$label))))]>;
def : Semantics<LDRXl, [(set GPR64:$Rt, (load (i64 (bitconvert am_ldrlit:$label))))]>;
def : Semantics<LDRSl, [(set (f32 FPR32:$Rt), (load (i64 (bitconvert am_ldrlit:$label))))]>;
def : Semantics<LDRDl, [(set (f64 FPR64:$Rt), (load (i64 (bitconvert am_ldrlit:$label))))]>;
def : Semantics<LDRQl, [(set (f128 FPR128:$Rt), (load (i64 (bitconvert am_ldrlit:$label))))]>;


def : Semantics<SUBWri, [(set GPR32sp:$Rd, (sub GPR32sp:$Rn, addsub_shifted_imm32:$imm))]>;

//Misc

def : Semantics<ADR, [(set GPR64:$Xd, adrlabel:$label)]>;


//FP

class Sema_FMOV<Instruction inst, ValueType DstType, DAGOperand DstOperand, ValueType SrcType, DAGOperand SrcOperand> : Semantics<inst, [(set (DstType DstOperand:$Rd), (bitconvert (SrcType SrcOperand:$Rn)))]>;

def : Sema_FMOV<FMOVXDr, f64, FPR64, i64, GPR64>;
def : Sema_FMOV<FMOVWSr, f32, FPR32, i32, GPR32>;
def : Sema_FMOV<FMOVDXr, i64, GPR64, f64, FPR64>;
def : Sema_FMOV<FMOVSWr, i32, GPR32, f32, FPR32>;
def : Sema_FMOV<FMOVSr, f32, FPR32, f32, FPR32>;
def : Semantics<FMOVDr, [(set (f64 FPR64:$Rd), (f64 FPR64:$Rn))]>;

//def : Semantics<SCVTFv1i64, [(set (f64 FPR64:$Rd), (bitconvert (AArch64sitof (i64 (bitconvert (f64 FPR64:$Rn))))))]>;

def : Semantics<HINT, [(set WZR, imm0_127:$imm)]>;
def : Semantics<BRK, [(set WZR, imm0_65535:$imm)]>;



//VECTOR

def : Semantics<INSvi8gpr, [(set V128:$dst, (vector_insert (v16i8 V128:$Rd), (i8 (trunc GPR32:$Rn)), VectorIndexB:$idx))]>;
def : Semantics<INSvi16gpr, [(set V128:$dst, (vector_insert (v8i16 V128:$Rd), (i16 (trunc GPR32:$Rn)), VectorIndexH:$idx))]>;

//LDAR STLR

def : Semantics<LDARW, [(set GPR32:$Rt, (load GPR64sp0:$Rn))]>;
def : Semantics<LDARX, [(set GPR64:$Rt, (load GPR64sp0:$Rn))]>;
def : Semantics<LDARB, [(set GPR32:$Rt, (zext (i8 (load GPR64sp0:$Rn))))]>;
def : Semantics<LDARH, [(set GPR32:$Rt, (zext (i16 (load GPR64sp0:$Rn))))]>;

def : Semantics<LDXRW, [(set GPR32:$Rt, (load GPR64sp0:$Rn))]>;
def : Semantics<LDXRX, [(set GPR64:$Rt, (load GPR64sp0:$Rn))]>;
def : Semantics<LDXRB, [(set GPR32:$Rt, (zext (i8 (load GPR64sp0:$Rn))))]>;
def : Semantics<LDXRH, [(set GPR32:$Rt, (zext (i16 (load GPR64sp0:$Rn))))]>;

def : Semantics<STLRB , [(store (i8 (trunc GPR32:$Rt)), GPR64sp0:$Rn)]>;
def : Semantics<STLRH , [(store (i16 (trunc GPR32:$Rt)), GPR64sp0:$Rn)]>;
def : Semantics<STLRW , [(store GPR32:$Rt, GPR64sp0:$Rn)]>;
def : Semantics<STLRX , [(store GPR64:$Rt, GPR64sp0:$Rn)]>;

def : Semantics<STXRB, [(set GPR32:$Ws, 0), (store (i8 (trunc GPR32:$Rt)), GPR64sp0:$Rn)]>;
def : Semantics<STXRH, [(set GPR32:$Ws, 0), (store (i16 (trunc GPR32:$Rt)), GPR64sp0:$Rn)]>;
def : Semantics<STXRW, [(set GPR32:$Ws, 0), (store GPR32:$Rt, GPR64sp0:$Rn)]>;
def : Semantics<STXRX, [(set GPR32:$Ws, 0), (store GPR64:$Rt, GPR64sp0:$Rn)]>;

///////

def : Semantics<BSLv16i8, [(set (v16i8 V128:$dst), (or (and (v16i8 V128:$Rd), (v16i8 V128:$Rn)), (and (vnot (v16i8 V128:$Rd)), (v16i8 V128:$Rm))))]>;





//XTN

def : Semantics<XTNv4i32, [(set (v4i32 V128:$dst), (bitconvert (or (srl (i128 (shl (i128 (bitconvert (v4i32 V128:$Rd))), (i128 64))), (i128 64)), (i128 (shl (i128 (zext (i64 (bitconvert (v2i32 (trunc (v2i64 V128:$Rn))))))), (i128 64))))))]>;
def : Semantics<XTNv8i16, [(set (v8i16 V128:$dst), (bitconvert (or (srl (i128 (shl (i128 (bitconvert (v8i16 V128:$Rd))), (i128 64))), (i128 64)), (i128 (shl (i128 (zext (i64 (bitconvert (v4i16 (trunc (v4i32 V128:$Rn))))))), (i128 64))))))]>;
def : Semantics<XTNv16i8, [(set (v16i8 V128:$dst), (bitconvert (or (srl (i128 (shl (i128 (bitconvert (v16i8 V128:$Rd))), (i128 64))), (i128 64)), (i128 (shl (i128 (zext (i64 (bitconvert (v8i8 (trunc (v8i16 V128:$Rn))))))), (i128 64))))))]>;

//def : Semantics<SSHLLv2i32_shift, [(set V128:$Rd, )]>;


def : Semantics<LDRHHpre, [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set GPR32:$Rt, (zext (i16 (load GPR64sp:$wback))))]>;

def : Semantics<MOVIv4i32, [(set (v4i32 V128:$Rd), (AArch64movi (shl imm0_255:$imm8, logical_vec_shift:$shift)))]>;
def : Semantics<MOVIv8i16, [(set (v8i16 V128:$Rd), (AArch64movi (shl imm0_255:$imm8, logical_vec_hw_shift:$shift)))]>;

def : Semantics<RBITXr, [(set GPR64:$Rd, (int_aarch64_rbit GPR64:$Rn))]>;
def : Semantics<RBITWr, [(set GPR32:$Rd, (int_aarch64_rbit GPR32:$Rn))]>;

def : Semantics<CPYi8, [(set FPR8:$dst, (bitconvert (i8 (vector_extract (v16i8 V128:$src), VectorIndexB:$idx))))]>;
def : Semantics<CPYi16, [(set FPR16:$dst, (f16 (vector_extract (v8f16 V128:$src), VectorIndexH:$idx)))]>;
def : Semantics<CPYi32, [(set FPR32:$dst, (f32 (vector_extract (v4f32 V128:$src), VectorIndexS:$idx)))]>;
def : Semantics<CPYi64, [(set FPR64:$dst, (f64 (vector_extract (v2f64 V128:$src), VectorIndexD:$idx)))]>;

def : Semantics<SQSHRNv16i8_shift, [(set (v16i8 V128:$dst), (v16i8 (bitconvert (or (shl (i128 (zext (i64 (bitconvert (v8i8 (int_aarch64_neon_sqshrn (v8i16 V128:$Rn), vecshiftR16Narrow:$imm)))))), (i128 64)), (i128 (bitconvert (v16i8 V128:$Rd)))))))]>;


def : Semantics<MVNIv8i16, [(set (v8i16 V128:$Rd), (AArch64mvni_shift imm0_255:$imm8, (i32 imm:$shift)))]>;
def : Semantics<MOVIv4i16, [(set (v4i16 V64:$Rd), (AArch64movi_shift imm0_255:$imm8, (i32 imm:$shift)))]>;
def : Semantics<MOVIv2i32, [(set (v2i32 V64:$Rd), (AArch64movi_shift imm0_255:$imm8, (i32 imm:$shift)))]>;
def : Semantics<MVNIv4i16, [(set (v4i16 V64:$Rd), (AArch64mvni_shift imm0_255:$imm8, (i32 imm:$shift)))]>;
def : Semantics<MVNIv4i32, [(set (v4i32 V128:$Rd), (AArch64mvni_shift imm0_255:$imm8, (i32 imm:$shift)))]>;

def : Semantics<RSHRNv8i16_shift, [(set (v8i16 V128:$dst), (v8i16 (bitconvert (or (i128 (bitconvert (v4i32 V128:$Rd))), (shl (i128 (zext (i64 (bitconvert (v4i16 (int_aarch64_neon_rshrn (v4i32 V128:$Rn), vecshiftR32Narrow:$imm)))))), (i128 64))))))]>;

def : Semantics<SMOVvi32to64, [(set (i64 GPR64:$Rd), (i64 (sext (vector_extract (v4i32 V128:$Rn), VectorIndexS:$idx))))]>;


def : Semantics<STRSpre, [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (store (f32 FPR32:$Rt), GPR64sp:$Rn)]>;

def : Semantics<LDRSHXpre, [(set GPR64sp:$wback, (add GPR64sp:$Rn, simm9:$offset)), (set GPR64:$Rt, (sext (i16 (load GPR64sp:$Rn))))]>;


multiclass STLine<string inst, DAGOperand loadOperand, ValueType loadType, DAGOperand incrOperand> {
    def : Semantics<!cast<Instruction>(inst), [(store  (loadType (bitconvert loadOperand:$Vt)), GPR64sp:$Rn)]>;
    def : Semantics<!cast<Instruction>(inst#"_POST"), [(store  (loadType (bitconvert loadOperand:$Vt)), GPR64sp:$Rn), (set GPR64sp:$wback, (add GPR64sp:$Rn, incrOperand:$Xm))]>;
}

//defm : STLine<"ST2Twov2d", VecListTwo2d, v4i64, GPR64pi32>;
//defm : STLine<"ST2Twov2s", VecListTwo2s, v4i32, GPR64pi16>;
//defm : STLine<"ST2Twov4s", VecListTwo4s, v8i32, GPR64pi32>;
//defm : STLine<"ST2Twov4h", VecListTwo4h, v8i16, GPR64pi16>;
//defm : STLine<"ST2Twov8h", VecListTwo8h, v16i16, GPR64pi32>;
//defm : STLine<"ST2Twov8b", VecListTwo8b, v16i8, GPR64pi16>;
//defm : STLine<"ST2Twov16b", VecListTwo16b, v32i8, GPR64pi32>;



//multiclass foobar {
//def : Semantics<ST2Twov2d, [(store  (v4i64 (bitconvert VecListTwo2d:$Vt)), GPR64sp:$Rn)]>;
//def : Semantics<ST2Twov8b, [(store  (v16i8 (bitconvert VecListTwo8b:$Vt)), GPR64sp:$Rn)]>;
//def : Semantics<LD2Twov2d, [(set VecListTwo2d:$Vt, (bitconvert (v4i64 (load GPR64sp:$Rn))))]>;
//}
