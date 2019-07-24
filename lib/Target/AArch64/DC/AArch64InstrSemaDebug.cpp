#include "AArch64InstrSema.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringExtras.h"
#include "../AArch64.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

#define DEBUG_TYPE "dc-sema"

void AArch64InstrSema::printInstruction() {
    uint64_t  Address = CurrentInst->Address;
    DEBUG(CurrentInst->Inst.dump());
    switch (CurrentInst->Inst.getOpcode()) {
        default: {
            llvm_unreachable("unknown opcode");
        }
        case AArch64::PHI: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PHI\n");
            break;
        }
        case AArch64::INLINEASM: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INLINEASM\n");
            break;
        }
        case AArch64::CFI_INSTRUCTION: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CFI_INSTRUCTION\n");
            break;
        }
        case AArch64::EH_LABEL: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EH_LABEL\n");
            break;
        }
        case AArch64::GC_LABEL: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " GC_LABEL\n");
            break;
        }
        case AArch64::KILL: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " KILL\n");
            break;
        }
        case AArch64::EXTRACT_SUBREG: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EXTRACT_SUBREG\n");
            break;
        }
        case AArch64::INSERT_SUBREG: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSERT_SUBREG\n");
            break;
        }
        case AArch64::IMPLICIT_DEF: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " IMPLICIT_DEF\n");
            break;
        }
        case AArch64::SUBREG_TO_REG: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBREG_TO_REG\n");
            break;
        }
        case AArch64::COPY_TO_REGCLASS: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " COPY_TO_REGCLASS\n");
            break;
        }
        case AArch64::DBG_VALUE: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DBG_VALUE\n");
            break;
        }
        case AArch64::REG_SEQUENCE: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REG_SEQUENCE\n");
            break;
        }
        case AArch64::COPY: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " COPY\n");
            break;
        }
        case AArch64::BUNDLE: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BUNDLE\n");
            break;
        }
        case AArch64::LIFETIME_START: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LIFETIME_START\n");
            break;
        }
        case AArch64::LIFETIME_END: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LIFETIME_END\n");
            break;
        }
        case AArch64::STACKMAP: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STACKMAP\n");
            break;
        }
        case AArch64::PATCHPOINT: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PATCHPOINT\n");
            break;
        }
        case AArch64::LOAD_STACK_GUARD: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LOAD_STACK_GUARD\n");
            break;
        }
        case AArch64::STATEPOINT: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STATEPOINT\n");
            break;
        }
        case AArch64::LOCAL_ESCAPE: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LOCAL_ESCAPE\n");
            break;
        }
        case AArch64::FAULTING_LOAD_OP: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FAULTING_LOAD_OP\n");
            break;
        }
        case AArch64::ABSv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv16i8\n");
            break;
        }
        case AArch64::ABSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv1i64\n");
            break;
        }
        case AArch64::ABSv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv2i32\n");
            break;
        }
        case AArch64::ABSv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv2i64\n");
            break;
        }
        case AArch64::ABSv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv4i16\n");
            break;
        }
        case AArch64::ABSv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv4i32\n");
            break;
        }
        case AArch64::ABSv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv8i16\n");
            break;
        }
        case AArch64::ABSv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ABSv8i8\n");
            break;
        }
        case AArch64::ADCSWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADCSWr\n");
            break;
        }
        case AArch64::ADCSXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADCSXr\n");
            break;
        }
        case AArch64::ADCWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADCWr\n");
            break;
        }
        case AArch64::ADCXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADCXr\n");
            break;
        }
        case AArch64::ADDHNv2i64_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDHNv2i64_v2i32\n");
            break;
        }
        case AArch64::ADDHNv2i64_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDHNv2i64_v4i32\n");
            break;
        }
        case AArch64::ADDHNv4i32_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDHNv4i32_v4i16\n");
            break;
        }
        case AArch64::ADDHNv4i32_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDHNv4i32_v8i16\n");
            break;
        }
        case AArch64::ADDHNv8i16_v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDHNv8i16_v16i8\n");
            break;
        }
        case AArch64::ADDHNv8i16_v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDHNv8i16_v8i8\n");
            break;
        }
        case AArch64::ADDPv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv16i8\n");
            break;
        }
        case AArch64::ADDPv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv2i32\n");
            break;
        }
        case AArch64::ADDPv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv2i64\n");
            break;
        }
        case AArch64::ADDPv2i64p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv2i64p\n");
            break;
        }
        case AArch64::ADDPv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv4i16\n");
            break;
        }
        case AArch64::ADDPv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv4i32\n");
            break;
        }
        case AArch64::ADDPv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv8i16\n");
            break;
        }
        case AArch64::ADDPv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDPv8i8\n");
            break;
        }
        case AArch64::ADDSWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSWri\n");
            break;
        }
        case AArch64::ADDSWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSWrr\n");
            break;
        }
        case AArch64::ADDSWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSWrs\n");
            break;
        }
        case AArch64::ADDSWrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSWrx\n");
            break;
        }
        case AArch64::ADDSXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSXri\n");
            break;
        }
        case AArch64::ADDSXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSXrr\n");
            break;
        }
        case AArch64::ADDSXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSXrs\n");
            break;
        }
        case AArch64::ADDSXrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSXrx\n");
            break;
        }
        case AArch64::ADDSXrx64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDSXrx64\n");
            break;
        }
        case AArch64::ADDVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDVv16i8v\n");
            break;
        }
        case AArch64::ADDVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDVv4i16v\n");
            break;
        }
        case AArch64::ADDVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDVv4i32v\n");
            break;
        }
        case AArch64::ADDVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDVv8i16v\n");
            break;
        }
        case AArch64::ADDVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDVv8i8v\n");
            break;
        }
        case AArch64::ADDWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDWri\n");
            break;
        }
        case AArch64::ADDWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDWrr\n");
            break;
        }
        case AArch64::ADDWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDWrs\n");
            break;
        }
        case AArch64::ADDWrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDWrx\n");
            break;
        }
        case AArch64::ADDXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDXri\n");
            break;
        }
        case AArch64::ADDXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDXrr\n");
            break;
        }
        case AArch64::ADDXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDXrs\n");
            break;
        }
        case AArch64::ADDXrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDXrx\n");
            break;
        }
        case AArch64::ADDXrx64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDXrx64\n");
            break;
        }
        case AArch64::ADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv16i8\n");
            break;
        }
        case AArch64::ADDv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv1i64\n");
            break;
        }
        case AArch64::ADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv2i32\n");
            break;
        }
        case AArch64::ADDv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv2i64\n");
            break;
        }
        case AArch64::ADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv4i16\n");
            break;
        }
        case AArch64::ADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv4i32\n");
            break;
        }
        case AArch64::ADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv8i16\n");
            break;
        }
        case AArch64::ADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADDv8i8\n");
            break;
        }
        case AArch64::ADJCALLSTACKDOWN: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADJCALLSTACKDOWN\n");
            break;
        }
        case AArch64::ADJCALLSTACKUP: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADJCALLSTACKUP\n");
            break;
        }
        case AArch64::ADR: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADR\n");
            break;
        }
        case AArch64::ADRP: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ADRP\n");
            break;
        }
        case AArch64::AESDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " AESDrr\n");
            break;
        }
        case AArch64::AESErr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " AESErr\n");
            break;
        }
        case AArch64::AESIMCrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " AESIMCrr\n");
            break;
        }
        case AArch64::AESMCrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " AESMCrr\n");
            break;
        }
        case AArch64::ANDSWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDSWri\n");
            break;
        }
        case AArch64::ANDSWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDSWrr\n");
            break;
        }
        case AArch64::ANDSWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDSWrs\n");
            break;
        }
        case AArch64::ANDSXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDSXri\n");
            break;
        }
        case AArch64::ANDSXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDSXrr\n");
            break;
        }
        case AArch64::ANDSXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDSXrs\n");
            break;
        }
        case AArch64::ANDWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDWri\n");
            break;
        }
        case AArch64::ANDWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDWrr\n");
            break;
        }
        case AArch64::ANDWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDWrs\n");
            break;
        }
        case AArch64::ANDXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDXri\n");
            break;
        }
        case AArch64::ANDXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDXrr\n");
            break;
        }
        case AArch64::ANDXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDXrs\n");
            break;
        }
        case AArch64::ANDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDv16i8\n");
            break;
        }
        case AArch64::ANDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ANDv8i8\n");
            break;
        }
        case AArch64::ASRVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ASRVWr\n");
            break;
        }
        case AArch64::ASRVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ASRVXr\n");
            break;
        }
        case AArch64::B: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " B\n");
            break;
        }
        case AArch64::BFMWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BFMWri\n");
            break;
        }
        case AArch64::BFMXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BFMXri\n");
            break;
        }
        case AArch64::BICSWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICSWrr\n");
            break;
        }
        case AArch64::BICSWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICSWrs\n");
            break;
        }
        case AArch64::BICSXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICSXrr\n");
            break;
        }
        case AArch64::BICSXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICSXrs\n");
            break;
        }
        case AArch64::BICWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICWrr\n");
            break;
        }
        case AArch64::BICWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICWrs\n");
            break;
        }
        case AArch64::BICXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICXrr\n");
            break;
        }
        case AArch64::BICXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICXrs\n");
            break;
        }
        case AArch64::BICv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICv16i8\n");
            break;
        }
        case AArch64::BICv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICv2i32\n");
            break;
        }
        case AArch64::BICv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICv4i16\n");
            break;
        }
        case AArch64::BICv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICv4i32\n");
            break;
        }
        case AArch64::BICv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICv8i16\n");
            break;
        }
        case AArch64::BICv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BICv8i8\n");
            break;
        }
        case AArch64::BIFv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BIFv16i8\n");
            break;
        }
        case AArch64::BIFv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BIFv8i8\n");
            break;
        }
        case AArch64::BITv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BITv16i8\n");
            break;
        }
        case AArch64::BITv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BITv8i8\n");
            break;
        }
        case AArch64::BL: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BL\n");
            break;
        }
        case AArch64::BLR: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BLR\n");
            break;
        }
        case AArch64::BR: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BR\n");
            break;
        }
        case AArch64::BRK: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BRK\n");
            break;
        }
        case AArch64::BSLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BSLv16i8\n");
            break;
        }
        case AArch64::BSLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " BSLv8i8\n");
            break;
        }
        case AArch64::Bcc: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " Bcc\n");
            break;
        }
        case AArch64::CASALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASALb\n");
            break;
        }
        case AArch64::CASALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASALd\n");
            break;
        }
        case AArch64::CASALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASALh\n");
            break;
        }
        case AArch64::CASALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASALs\n");
            break;
        }
        case AArch64::CASAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASAb\n");
            break;
        }
        case AArch64::CASAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASAd\n");
            break;
        }
        case AArch64::CASAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASAh\n");
            break;
        }
        case AArch64::CASAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASAs\n");
            break;
        }
        case AArch64::CASLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASLb\n");
            break;
        }
        case AArch64::CASLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASLd\n");
            break;
        }
        case AArch64::CASLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASLh\n");
            break;
        }
        case AArch64::CASLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASLs\n");
            break;
        }
//        case AArch64::CASPALd: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPALd\n");
//            break;
//        }
//        case AArch64::CASPALs: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPALs\n");
//            break;
//        }
//        case AArch64::CASPAd: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPAd\n");
//            break;
//        }
//        case AArch64::CASPAs: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPAs\n");
//            break;//        }
//        case AArch64::CASPLd: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPLd\n");
//            break;
//        }
//        case AArch64::CASPLs: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPLs\n");
//            break;
//        }
//        case AArch64::CASPd: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPd\n");
//            break;
//        }
//        case AArch64::CASPs: {
//            DEBUG(errs() << "0x" << utohexstr(Address) << " CASPs\n");
//            break;
//        }
        case AArch64::CASb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASb\n");
            break;
        }
        case AArch64::CASd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASd\n");
            break;
        }
        case AArch64::CASh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASh\n");
            break;
        }
        case AArch64::CASs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CASs\n");
            break;
        }
        case AArch64::CBNZW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CBNZW\n");
            break;
        }
        case AArch64::CBNZX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CBNZX\n");
            break;
        }
        case AArch64::CBZW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CBZW\n");
            break;
        }
        case AArch64::CBZX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CBZX\n");
            break;
        }
        case AArch64::CCMNWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMNWi\n");
            break;
        }
        case AArch64::CCMNWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMNWr\n");
            break;
        }
        case AArch64::CCMNXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMNXi\n");
            break;
        }
        case AArch64::CCMNXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMNXr\n");
            break;
        }
        case AArch64::CCMPWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMPWi\n");
            break;
        }
        case AArch64::CCMPWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMPWr\n");
            break;
        }
        case AArch64::CCMPXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMPXi\n");
            break;
        }
        case AArch64::CCMPXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CCMPXr\n");
            break;
        }
        case AArch64::CLREX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLREX\n");
            break;
        }
        case AArch64::CLSWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSWr\n");
            break;
        }
        case AArch64::CLSXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSXr\n");
            break;
        }
        case AArch64::CLSv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSv16i8\n");
            break;
        }
        case AArch64::CLSv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSv2i32\n");
            break;
        }
        case AArch64::CLSv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSv4i16\n");
            break;
        }
        case AArch64::CLSv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSv4i32\n");
            break;
        }
        case AArch64::CLSv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSv8i16\n");
            break;
        }
        case AArch64::CLSv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLSv8i8\n");
            break;
        }
        case AArch64::CLZWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZWr\n");
            break;
        }
        case AArch64::CLZXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZXr\n");
            break;
        }
        case AArch64::CLZv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZv16i8\n");
            break;
        }
        case AArch64::CLZv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZv2i32\n");
            break;
        }
        case AArch64::CLZv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZv4i16\n");
            break;
        }
        case AArch64::CLZv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZv4i32\n");
            break;
        }
        case AArch64::CLZv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZv8i16\n");
            break;
        }
        case AArch64::CLZv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CLZv8i8\n");
            break;
        }
        case AArch64::CMEQv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv16i8\n");
            break;
        }
        case AArch64::CMEQv16i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv16i8rz\n");
            break;
        }
        case AArch64::CMEQv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv1i64\n");
            break;
        }
        case AArch64::CMEQv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv1i64rz\n");
            break;
        }
        case AArch64::CMEQv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv2i32\n");
            break;
        }
        case AArch64::CMEQv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv2i32rz\n");
            break;
        }
        case AArch64::CMEQv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv2i64\n");
            break;
        }
        case AArch64::CMEQv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv2i64rz\n");
            break;
        }
        case AArch64::CMEQv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv4i16\n");
            break;
        }
        case AArch64::CMEQv4i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv4i16rz\n");
            break;
        }
        case AArch64::CMEQv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv4i32\n");
            break;
        }
        case AArch64::CMEQv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv4i32rz\n");
            break;
        }
        case AArch64::CMEQv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv8i16\n");
            break;
        }
        case AArch64::CMEQv8i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv8i16rz\n");
            break;
        }
        case AArch64::CMEQv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv8i8\n");
            break;
        }
        case AArch64::CMEQv8i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMEQv8i8rz\n");
            break;
        }
        case AArch64::CMGEv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv16i8\n");
            break;
        }
        case AArch64::CMGEv16i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv16i8rz\n");
            break;
        }
        case AArch64::CMGEv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv1i64\n");
            break;
        }
        case AArch64::CMGEv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv1i64rz\n");
            break;
        }
        case AArch64::CMGEv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv2i32\n");
            break;
        }
        case AArch64::CMGEv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv2i32rz\n");
            break;
        }
        case AArch64::CMGEv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv2i64\n");
            break;
        }
        case AArch64::CMGEv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv2i64rz\n");
            break;
        }
        case AArch64::CMGEv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv4i16\n");
            break;
        }
        case AArch64::CMGEv4i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv4i16rz\n");
            break;
        }
        case AArch64::CMGEv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv4i32\n");
            break;
        }
        case AArch64::CMGEv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv4i32rz\n");
            break;
        }
        case AArch64::CMGEv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv8i16\n");
            break;
        }
        case AArch64::CMGEv8i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv8i16rz\n");
            break;
        }
        case AArch64::CMGEv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv8i8\n");
            break;
        }
        case AArch64::CMGEv8i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGEv8i8rz\n");
            break;
        }
        case AArch64::CMGTv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv16i8\n");
            break;
        }
        case AArch64::CMGTv16i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv16i8rz\n");
            break;
        }
        case AArch64::CMGTv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv1i64\n");
            break;
        }
        case AArch64::CMGTv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv1i64rz\n");
            break;
        }
        case AArch64::CMGTv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv2i32\n");
            break;
        }
        case AArch64::CMGTv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv2i32rz\n");
            break;
        }
        case AArch64::CMGTv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv2i64\n");
            break;
        }
        case AArch64::CMGTv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv2i64rz\n");
            break;
        }
        case AArch64::CMGTv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv4i16\n");
            break;
        }
        case AArch64::CMGTv4i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv4i16rz\n");
            break;
        }
        case AArch64::CMGTv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv4i32\n");
            break;
        }
        case AArch64::CMGTv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv4i32rz\n");
            break;
        }
        case AArch64::CMGTv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv8i16\n");
            break;
        }
        case AArch64::CMGTv8i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv8i16rz\n");
            break;
        }
        case AArch64::CMGTv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv8i8\n");
            break;
        }
        case AArch64::CMGTv8i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMGTv8i8rz\n");
            break;
        }
        case AArch64::CMHIv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv16i8\n");
            break;
        }
        case AArch64::CMHIv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv1i64\n");
            break;
        }
        case AArch64::CMHIv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv2i32\n");
            break;
        }
        case AArch64::CMHIv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv2i64\n");
            break;
        }
        case AArch64::CMHIv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv4i16\n");
            break;
        }
        case AArch64::CMHIv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv4i32\n");
            break;
        }
        case AArch64::CMHIv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv8i16\n");
            break;
        }
        case AArch64::CMHIv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHIv8i8\n");
            break;
        }
        case AArch64::CMHSv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv16i8\n");
            break;
        }
        case AArch64::CMHSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv1i64\n");
            break;
        }
        case AArch64::CMHSv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv2i32\n");
            break;
        }
        case AArch64::CMHSv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv2i64\n");
            break;
        }
        case AArch64::CMHSv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv4i16\n");
            break;
        }
        case AArch64::CMHSv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv4i32\n");
            break;
        }
        case AArch64::CMHSv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv8i16\n");
            break;
        }
        case AArch64::CMHSv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMHSv8i8\n");
            break;
        }
        case AArch64::CMLEv16i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv16i8rz\n");
            break;
        }
        case AArch64::CMLEv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv1i64rz\n");
            break;
        }
        case AArch64::CMLEv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv2i32rz\n");
            break;
        }
        case AArch64::CMLEv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv2i64rz\n");
            break;
        }
        case AArch64::CMLEv4i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv4i16rz\n");
            break;
        }
        case AArch64::CMLEv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv4i32rz\n");
            break;
        }
        case AArch64::CMLEv8i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv8i16rz\n");
            break;
        }
        case AArch64::CMLEv8i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLEv8i8rz\n");
            break;
        }
        case AArch64::CMLTv16i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv16i8rz\n");
            break;
        }
        case AArch64::CMLTv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv1i64rz\n");
            break;
        }
        case AArch64::CMLTv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv2i32rz\n");
            break;
        }
        case AArch64::CMLTv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv2i64rz\n");
            break;
        }
        case AArch64::CMLTv4i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv4i16rz\n");
            break;
        }
        case AArch64::CMLTv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv4i32rz\n");
            break;
        }
        case AArch64::CMLTv8i16rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv8i16rz\n");
            break;
        }
        case AArch64::CMLTv8i8rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMLTv8i8rz\n");
            break;
        }
        case AArch64::CMTSTv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv16i8\n");
            break;
        }
        case AArch64::CMTSTv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv1i64\n");
            break;
        }
        case AArch64::CMTSTv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv2i32\n");
            break;
        }
        case AArch64::CMTSTv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv2i64\n");
            break;
        }
        case AArch64::CMTSTv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv4i16\n");
            break;
        }
        case AArch64::CMTSTv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv4i32\n");
            break;
        }
        case AArch64::CMTSTv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv8i16\n");
            break;
        }
        case AArch64::CMTSTv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CMTSTv8i8\n");
            break;
        }
        case AArch64::CNTv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CNTv16i8\n");
            break;
        }
        case AArch64::CNTv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CNTv8i8\n");
            break;
        }
        case AArch64::CPYi16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CPYi16\n");
            break;
        }
        case AArch64::CPYi32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CPYi32\n");
            break;
        }
        case AArch64::CPYi64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CPYi64\n");
            break;
        }
        case AArch64::CPYi8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CPYi8\n");
            break;
        }
        case AArch64::CRC32Brr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32Brr\n");
            break;
        }
        case AArch64::CRC32CBrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32CBrr\n");
            break;
        }
        case AArch64::CRC32CHrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32CHrr\n");
            break;
        }
        case AArch64::CRC32CWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32CWrr\n");
            break;
        }
        case AArch64::CRC32CXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32CXrr\n");
            break;
        }
        case AArch64::CRC32Hrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32Hrr\n");
            break;
        }
        case AArch64::CRC32Wrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32Wrr\n");
            break;
        }
        case AArch64::CRC32Xrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CRC32Xrr\n");
            break;
        }
        case AArch64::CSELWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSELWr\n");
            break;
        }
        case AArch64::CSELXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSELXr\n");
            break;
        }
        case AArch64::CSINCWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSINCWr\n");
            break;
        }
        case AArch64::CSINCXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSINCXr\n");
            break;
        }
        case AArch64::CSINVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSINVWr\n");
            break;
        }
        case AArch64::CSINVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSINVXr\n");
            break;
        }
        case AArch64::CSNEGWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSNEGWr\n");
            break;
        }
        case AArch64::CSNEGXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " CSNEGXr\n");
            break;
        }
        case AArch64::DCPS1: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DCPS1\n");
            break;
        }
        case AArch64::DCPS2: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DCPS2\n");
            break;
        }
        case AArch64::DCPS3: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DCPS3\n");
            break;
        }
        case AArch64::DMB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DMB\n");
            break;
        }
        case AArch64::DRPS: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DRPS\n");
            break;
        }
        case AArch64::DSB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DSB\n");
            break;
        }
        case AArch64::DUPv16i8gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv16i8gpr\n");
            break;
        }
        case AArch64::DUPv16i8lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv16i8lane\n");
            break;
        }
        case AArch64::DUPv2i32gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv2i32gpr\n");
            break;
        }
        case AArch64::DUPv2i32lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv2i32lane\n");
            break;
        }
        case AArch64::DUPv2i64gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv2i64gpr\n");
            break;
        }
        case AArch64::DUPv2i64lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv2i64lane\n");
            break;
        }
        case AArch64::DUPv4i16gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv4i16gpr\n");
            break;
        }
        case AArch64::DUPv4i16lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv4i16lane\n");
            break;
        }
        case AArch64::DUPv4i32gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv4i32gpr\n");
            break;
        }
        case AArch64::DUPv4i32lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv4i32lane\n");
            break;
        }
        case AArch64::DUPv8i16gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv8i16gpr\n");
            break;
        }
        case AArch64::DUPv8i16lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv8i16lane\n");
            break;
        }
        case AArch64::DUPv8i8gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv8i8gpr\n");
            break;
        }
        case AArch64::DUPv8i8lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " DUPv8i8lane\n");
            break;
        }
        case AArch64::EONWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EONWrr\n");
            break;
        }
        case AArch64::EONWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EONWrs\n");
            break;
        }
        case AArch64::EONXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EONXrr\n");
            break;
        }
        case AArch64::EONXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EONXrs\n");
            break;
        }
        case AArch64::EORWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORWri\n");
            break;
        }
        case AArch64::EORWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORWrr\n");
            break;
        }
        case AArch64::EORWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORWrs\n");
            break;
        }
        case AArch64::EORXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORXri\n");
            break;
        }
        case AArch64::EORXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORXrr\n");
            break;
        }
        case AArch64::EORXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORXrs\n");
            break;
        }
        case AArch64::EORv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORv16i8\n");
            break;
        }
        case AArch64::EORv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EORv8i8\n");
            break;
        }
        case AArch64::ERET: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ERET\n");
            break;
        }
        case AArch64::EXTRWrri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EXTRWrri\n");
            break;
        }
        case AArch64::EXTRXrri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EXTRXrri\n");
            break;
        }
        case AArch64::EXTv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EXTv16i8\n");
            break;
        }
        case AArch64::EXTv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " EXTv8i8\n");
            break;
        }
        case AArch64::F128CSEL: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " F128CSEL\n");
            break;
        }
        case AArch64::FABD32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABD32\n");
            break;
        }
        case AArch64::FABD64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABD64\n");
            break;
        }
        case AArch64::FABDv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABDv2f32\n");
            break;
        }
        case AArch64::FABDv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABDv2f64\n");
            break;
        }
        case AArch64::FABDv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABDv4f32\n");
            break;
        }
        case AArch64::FABSDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABSDr\n");
            break;
        }
        case AArch64::FABSSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABSSr\n");
            break;
        }
        case AArch64::FABSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABSv2f32\n");
            break;
        }
        case AArch64::FABSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABSv2f64\n");
            break;
        }
        case AArch64::FABSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FABSv4f32\n");
            break;
        }
        case AArch64::FACGE32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGE32\n");
            break;
        }
        case AArch64::FACGE64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGE64\n");
            break;
        }
        case AArch64::FACGEv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGEv2f32\n");
            break;
        }
        case AArch64::FACGEv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGEv2f64\n");
            break;
        }
        case AArch64::FACGEv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGEv4f32\n");
            break;
        }
        case AArch64::FACGT32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGT32\n");
            break;
        }
        case AArch64::FACGT64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGT64\n");
            break;
        }
        case AArch64::FACGTv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGTv2f32\n");
            break;
        }
        case AArch64::FACGTv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGTv2f64\n");
            break;
        }
        case AArch64::FACGTv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FACGTv4f32\n");
            break;
        }
        case AArch64::FADDDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDDrr\n");
            break;
        }
        case AArch64::FADDPv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDPv2f32\n");
            break;
        }
        case AArch64::FADDPv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDPv2f64\n");
            break;
        }
        case AArch64::FADDPv2i32p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDPv2i32p\n");
            break;
        }
        case AArch64::FADDPv2i64p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDPv2i64p\n");
            break;
        }
        case AArch64::FADDPv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDPv4f32\n");
            break;
        }
        case AArch64::FADDSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDSrr\n");
            break;
        }
        case AArch64::FADDv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDv2f32\n");
            break;
        }
        case AArch64::FADDv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDv2f64\n");
            break;
        }
        case AArch64::FADDv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FADDv4f32\n");
            break;
        }
        case AArch64::FCCMPDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCCMPDrr\n");
            break;
        }
        case AArch64::FCCMPEDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCCMPEDrr\n");
            break;
        }
        case AArch64::FCCMPESrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCCMPESrr\n");
            break;
        }
        case AArch64::FCCMPSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCCMPSrr\n");
            break;
        }
        case AArch64::FCMEQ32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQ32\n");
            break;
        }
        case AArch64::FCMEQ64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQ64\n");
            break;
        }
        case AArch64::FCMEQv1i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv1i32rz\n");
            break;
        }
        case AArch64::FCMEQv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv1i64rz\n");
            break;
        }
        case AArch64::FCMEQv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv2f32\n");
            break;
        }
        case AArch64::FCMEQv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv2f64\n");
            break;
        }
        case AArch64::FCMEQv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv2i32rz\n");
            break;
        }
        case AArch64::FCMEQv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv2i64rz\n");
            break;
        }
        case AArch64::FCMEQv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv4f32\n");
            break;
        }
        case AArch64::FCMEQv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMEQv4i32rz\n");
            break;
        }
        case AArch64::FCMGE32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGE32\n");
            break;
        }
        case AArch64::FCMGE64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGE64\n");
            break;
        }
        case AArch64::FCMGEv1i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv1i32rz\n");
            break;
        }
        case AArch64::FCMGEv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv1i64rz\n");
            break;
        }
        case AArch64::FCMGEv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv2f32\n");
            break;
        }
        case AArch64::FCMGEv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv2f64\n");
            break;
        }
        case AArch64::FCMGEv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv2i32rz\n");
            break;
        }
        case AArch64::FCMGEv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv2i64rz\n");
            break;
        }
        case AArch64::FCMGEv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv4f32\n");
            break;
        }
        case AArch64::FCMGEv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGEv4i32rz\n");
            break;
        }
        case AArch64::FCMGT32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGT32\n");
            break;
        }
        case AArch64::FCMGT64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGT64\n");
            break;
        }
        case AArch64::FCMGTv1i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv1i32rz\n");
            break;
        }
        case AArch64::FCMGTv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv1i64rz\n");
            break;
        }
        case AArch64::FCMGTv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv2f32\n");
            break;
        }
        case AArch64::FCMGTv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv2f64\n");
            break;
        }
        case AArch64::FCMGTv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv2i32rz\n");
            break;
        }
        case AArch64::FCMGTv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv2i64rz\n");
            break;
        }
        case AArch64::FCMGTv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv4f32\n");
            break;
        }
        case AArch64::FCMGTv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMGTv4i32rz\n");
            break;
        }
        case AArch64::FCMLEv1i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLEv1i32rz\n");
            break;
        }
        case AArch64::FCMLEv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLEv1i64rz\n");
            break;
        }
        case AArch64::FCMLEv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLEv2i32rz\n");
            break;
        }
        case AArch64::FCMLEv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLEv2i64rz\n");
            break;
        }
        case AArch64::FCMLEv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLEv4i32rz\n");
            break;
        }
        case AArch64::FCMLTv1i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLTv1i32rz\n");
            break;
        }
        case AArch64::FCMLTv1i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLTv1i64rz\n");
            break;
        }
        case AArch64::FCMLTv2i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLTv2i32rz\n");
            break;
        }
        case AArch64::FCMLTv2i64rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLTv2i64rz\n");
            break;
        }
        case AArch64::FCMLTv4i32rz: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMLTv4i32rz\n");
            break;
        }
        case AArch64::FCMPDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPDri\n");
            break;
        }
        case AArch64::FCMPDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPDrr\n");
            break;
        }
        case AArch64::FCMPEDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPEDri\n");
            break;
        }
        case AArch64::FCMPEDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPEDrr\n");
            break;
        }
        case AArch64::FCMPESri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPESri\n");
            break;
        }
        case AArch64::FCMPESrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPESrr\n");
            break;
        }
        case AArch64::FCMPSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPSri\n");
            break;
        }
        case AArch64::FCMPSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCMPSrr\n");
            break;
        }
        case AArch64::FCSELDrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCSELDrrr\n");
            break;
        }
        case AArch64::FCSELSrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCSELSrrr\n");
            break;
        }
        case AArch64::FCVTASUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASUWDr\n");
            break;
        }
        case AArch64::FCVTASUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASUWSr\n");
            break;
        }
        case AArch64::FCVTASUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASUXDr\n");
            break;
        }
        case AArch64::FCVTASUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASUXSr\n");
            break;
        }
        case AArch64::FCVTASv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASv1i32\n");
            break;
        }
        case AArch64::FCVTASv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASv1i64\n");
            break;
        }
        case AArch64::FCVTASv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASv2f32\n");
            break;
        }
        case AArch64::FCVTASv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASv2f64\n");
            break;
        }
        case AArch64::FCVTASv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTASv4f32\n");
            break;
        }
        case AArch64::FCVTAUUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUUWDr\n");
            break;
        }
        case AArch64::FCVTAUUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUUWSr\n");
            break;
        }
        case AArch64::FCVTAUUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUUXDr\n");
            break;
        }
        case AArch64::FCVTAUUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUUXSr\n");
            break;
        }
        case AArch64::FCVTAUv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUv1i32\n");
            break;
        }
        case AArch64::FCVTAUv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUv1i64\n");
            break;
        }
        case AArch64::FCVTAUv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUv2f32\n");
            break;
        }
        case AArch64::FCVTAUv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUv2f64\n");
            break;
        }
        case AArch64::FCVTAUv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTAUv4f32\n");
            break;
        }
        case AArch64::FCVTDHr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTDHr\n");
            break;
        }
        case AArch64::FCVTDSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTDSr\n");
            break;
        }
        case AArch64::FCVTHDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTHDr\n");
            break;
        }
        case AArch64::FCVTHSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTHSr\n");
            break;
        }
        case AArch64::FCVTLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTLv2i32\n");
            break;
        }
        case AArch64::FCVTLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTLv4i16\n");
            break;
        }
        case AArch64::FCVTLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTLv4i32\n");
            break;
        }
        case AArch64::FCVTLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTLv8i16\n");
            break;
        }
        case AArch64::FCVTMSUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSUWDr\n");
            break;
        }
        case AArch64::FCVTMSUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSUWSr\n");
            break;
        }
        case AArch64::FCVTMSUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSUXDr\n");
            break;
        }
        case AArch64::FCVTMSUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSUXSr\n");
            break;
        }
        case AArch64::FCVTMSv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSv1i32\n");
            break;
        }
        case AArch64::FCVTMSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSv1i64\n");
            break;
        }
        case AArch64::FCVTMSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSv2f32\n");
            break;
        }
        case AArch64::FCVTMSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSv2f64\n");
            break;
        }
        case AArch64::FCVTMSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMSv4f32\n");
            break;
        }
        case AArch64::FCVTMUUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUUWDr\n");
            break;
        }
        case AArch64::FCVTMUUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUUWSr\n");
            break;
        }
        case AArch64::FCVTMUUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUUXDr\n");
            break;
        }
        case AArch64::FCVTMUUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUUXSr\n");
            break;
        }
        case AArch64::FCVTMUv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUv1i32\n");
            break;
        }
        case AArch64::FCVTMUv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUv1i64\n");
            break;
        }
        case AArch64::FCVTMUv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUv2f32\n");
            break;
        }
        case AArch64::FCVTMUv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUv2f64\n");
            break;
        }
        case AArch64::FCVTMUv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTMUv4f32\n");
            break;
        }
        case AArch64::FCVTNSUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSUWDr\n");
            break;
        }
        case AArch64::FCVTNSUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSUWSr\n");
            break;
        }
        case AArch64::FCVTNSUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSUXDr\n");
            break;
        }
        case AArch64::FCVTNSUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSUXSr\n");
            break;
        }
        case AArch64::FCVTNSv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSv1i32\n");
            break;
        }
        case AArch64::FCVTNSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSv1i64\n");
            break;
        }
        case AArch64::FCVTNSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSv2f32\n");
            break;
        }
        case AArch64::FCVTNSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSv2f64\n");
            break;
        }
        case AArch64::FCVTNSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNSv4f32\n");
            break;
        }
        case AArch64::FCVTNUUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUUWDr\n");
            break;
        }
        case AArch64::FCVTNUUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUUWSr\n");
            break;
        }
        case AArch64::FCVTNUUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUUXDr\n");
            break;
        }
        case AArch64::FCVTNUUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUUXSr\n");
            break;
        }
        case AArch64::FCVTNUv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUv1i32\n");
            break;
        }
        case AArch64::FCVTNUv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUv1i64\n");
            break;
        }
        case AArch64::FCVTNUv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUv2f32\n");
            break;
        }
        case AArch64::FCVTNUv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUv2f64\n");
            break;
        }
        case AArch64::FCVTNUv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNUv4f32\n");
            break;
        }
        case AArch64::FCVTNv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNv2i32\n");
            break;
        }
        case AArch64::FCVTNv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNv4i16\n");
            break;
        }
        case AArch64::FCVTNv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNv4i32\n");
            break;
        }
        case AArch64::FCVTNv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTNv8i16\n");
            break;
        }
        case AArch64::FCVTPSUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSUWDr\n");
            break;
        }
        case AArch64::FCVTPSUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSUWSr\n");
            break;
        }
        case AArch64::FCVTPSUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSUXDr\n");
            break;
        }
        case AArch64::FCVTPSUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSUXSr\n");
            break;
        }
        case AArch64::FCVTPSv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSv1i32\n");
            break;
        }
        case AArch64::FCVTPSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSv1i64\n");
            break;
        }
        case AArch64::FCVTPSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSv2f32\n");
            break;
        }
        case AArch64::FCVTPSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSv2f64\n");
            break;
        }
        case AArch64::FCVTPSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPSv4f32\n");
            break;
        }
        case AArch64::FCVTPUUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUUWDr\n");
            break;
        }
        case AArch64::FCVTPUUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUUWSr\n");
            break;
        }
        case AArch64::FCVTPUUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUUXDr\n");
            break;
        }
        case AArch64::FCVTPUUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUUXSr\n");
            break;
        }
        case AArch64::FCVTPUv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUv1i32\n");
            break;
        }
        case AArch64::FCVTPUv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUv1i64\n");
            break;
        }
        case AArch64::FCVTPUv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUv2f32\n");
            break;
        }
        case AArch64::FCVTPUv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUv2f64\n");
            break;
        }
        case AArch64::FCVTPUv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTPUv4f32\n");
            break;
        }
        case AArch64::FCVTSDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTSDr\n");
            break;
        }
        case AArch64::FCVTSHr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTSHr\n");
            break;
        }
        case AArch64::FCVTXNv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTXNv1i64\n");
            break;
        }
        case AArch64::FCVTXNv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTXNv2f32\n");
            break;
        }
        case AArch64::FCVTXNv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTXNv4f32\n");
            break;
        }
        case AArch64::FCVTZSSWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSSWDri\n");
            break;
        }
        case AArch64::FCVTZSSWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSSWSri\n");
            break;
        }
        case AArch64::FCVTZSSXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSSXDri\n");
            break;
        }
        case AArch64::FCVTZSSXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSSXSri\n");
            break;
        }
        case AArch64::FCVTZSUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSUWDr\n");
            break;
        }
        case AArch64::FCVTZSUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSUWSr\n");
            break;
        }
        case AArch64::FCVTZSUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSUXDr\n");
            break;
        }
        case AArch64::FCVTZSUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSUXSr\n");
            break;
        }
        case AArch64::FCVTZS_IntSWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntSWDri\n");
            break;
        }
        case AArch64::FCVTZS_IntSWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntSWSri\n");
            break;
        }
        case AArch64::FCVTZS_IntSXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntSXDri\n");
            break;
        }
        case AArch64::FCVTZS_IntSXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntSXSri\n");
            break;
        }
        case AArch64::FCVTZS_IntUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntUWDr\n");
            break;
        }
        case AArch64::FCVTZS_IntUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntUWSr\n");
            break;
        }
        case AArch64::FCVTZS_IntUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntUXDr\n");
            break;
        }
        case AArch64::FCVTZS_IntUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_IntUXSr\n");
            break;
        }
        case AArch64::FCVTZS_Intv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_Intv2f32\n");
            break;
        }
        case AArch64::FCVTZS_Intv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_Intv2f64\n");
            break;
        }
        case AArch64::FCVTZS_Intv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZS_Intv4f32\n");
            break;
        }
        case AArch64::FCVTZSd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSd\n");
            break;
        }
        case AArch64::FCVTZSs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSs\n");
            break;
        }
        case AArch64::FCVTZSv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv1i32\n");
            break;
        }
        case AArch64::FCVTZSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv1i64\n");
            break;
        }
        case AArch64::FCVTZSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv2f32\n");
            break;
        }
        case AArch64::FCVTZSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv2f64\n");
            break;
        }
        case AArch64::FCVTZSv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv2i32_shift\n");
            break;
        }
        case AArch64::FCVTZSv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv2i64_shift\n");
            break;
        }
        case AArch64::FCVTZSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv4f32\n");
            break;
        }
        case AArch64::FCVTZSv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZSv4i32_shift\n");
            break;
        }
        case AArch64::FCVTZUSWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUSWDri\n");
            break;
        }
        case AArch64::FCVTZUSWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUSWSri\n");
            break;
        }
        case AArch64::FCVTZUSXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUSXDri\n");
            break;
        }
        case AArch64::FCVTZUSXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUSXSri\n");
            break;
        }
        case AArch64::FCVTZUUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUUWDr\n");
            break;
        }
        case AArch64::FCVTZUUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUUWSr\n");
            break;
        }
        case AArch64::FCVTZUUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUUXDr\n");
            break;
        }
        case AArch64::FCVTZUUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUUXSr\n");
            break;
        }
        case AArch64::FCVTZU_IntSWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntSWDri\n");
            break;
        }
        case AArch64::FCVTZU_IntSWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntSWSri\n");
            break;
        }
        case AArch64::FCVTZU_IntSXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntSXDri\n");
            break;
        }
        case AArch64::FCVTZU_IntSXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntSXSri\n");
            break;
        }
        case AArch64::FCVTZU_IntUWDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntUWDr\n");
            break;
        }
        case AArch64::FCVTZU_IntUWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntUWSr\n");
            break;
        }
        case AArch64::FCVTZU_IntUXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntUXDr\n");
            break;
        }
        case AArch64::FCVTZU_IntUXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_IntUXSr\n");
            break;
        }
        case AArch64::FCVTZU_Intv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_Intv2f32\n");
            break;
        }
        case AArch64::FCVTZU_Intv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_Intv2f64\n");
            break;
        }
        case AArch64::FCVTZU_Intv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZU_Intv4f32\n");
            break;
        }
        case AArch64::FCVTZUd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUd\n");
            break;
        }
        case AArch64::FCVTZUs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUs\n");
            break;
        }
        case AArch64::FCVTZUv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv1i32\n");
            break;
        }
        case AArch64::FCVTZUv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv1i64\n");
            break;
        }
        case AArch64::FCVTZUv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv2f32\n");
            break;
        }
        case AArch64::FCVTZUv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv2f64\n");
            break;
        }
        case AArch64::FCVTZUv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv2i32_shift\n");
            break;
        }
        case AArch64::FCVTZUv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv2i64_shift\n");
            break;
        }
        case AArch64::FCVTZUv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv4f32\n");
            break;
        }
        case AArch64::FCVTZUv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FCVTZUv4i32_shift\n");
            break;
        }
        case AArch64::FDIVDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FDIVDrr\n");
            break;
        }
        case AArch64::FDIVSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FDIVSrr\n");
            break;
        }
        case AArch64::FDIVv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FDIVv2f32\n");
            break;
        }
        case AArch64::FDIVv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FDIVv2f64\n");
            break;
        }
        case AArch64::FDIVv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FDIVv4f32\n");
            break;
        }
        case AArch64::FMADDDrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMADDDrrr\n");
            break;
        }
        case AArch64::FMADDSrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMADDSrrr\n");
            break;
        }
        case AArch64::FMAXDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXDrr\n");
            break;
        }
        case AArch64::FMAXNMDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMDrr\n");
            break;
        }
        case AArch64::FMAXNMPv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMPv2f32\n");
            break;
        }
        case AArch64::FMAXNMPv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMPv2f64\n");
            break;
        }
        case AArch64::FMAXNMPv2i32p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMPv2i32p\n");
            break;
        }
        case AArch64::FMAXNMPv2i64p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMPv2i64p\n");
            break;
        }
        case AArch64::FMAXNMPv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMPv4f32\n");
            break;
        }
        case AArch64::FMAXNMSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMSrr\n");
            break;
        }
        case AArch64::FMAXNMVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMVv4i32v\n");
            break;
        }
        case AArch64::FMAXNMv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMv2f32\n");
            break;
        }
        case AArch64::FMAXNMv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMv2f64\n");
            break;
        }
        case AArch64::FMAXNMv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXNMv4f32\n");
            break;
        }
        case AArch64::FMAXPv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXPv2f32\n");
            break;
        }
        case AArch64::FMAXPv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXPv2f64\n");
            break;
        }
        case AArch64::FMAXPv2i32p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXPv2i32p\n");
            break;
        }
        case AArch64::FMAXPv2i64p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXPv2i64p\n");
            break;
        }
        case AArch64::FMAXPv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXPv4f32\n");
            break;
        }
        case AArch64::FMAXSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXSrr\n");
            break;
        }
        case AArch64::FMAXVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXVv4i32v\n");
            break;
        }
        case AArch64::FMAXv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXv2f32\n");
            break;
        }
        case AArch64::FMAXv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXv2f64\n");
            break;
        }
        case AArch64::FMAXv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMAXv4f32\n");
            break;
        }
        case AArch64::FMINDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINDrr\n");
            break;
        }
        case AArch64::FMINNMDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMDrr\n");
            break;
        }
        case AArch64::FMINNMPv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMPv2f32\n");
            break;
        }
        case AArch64::FMINNMPv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMPv2f64\n");
            break;
        }
        case AArch64::FMINNMPv2i32p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMPv2i32p\n");
            break;
        }
        case AArch64::FMINNMPv2i64p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMPv2i64p\n");
            break;
        }
        case AArch64::FMINNMPv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMPv4f32\n");
            break;
        }
        case AArch64::FMINNMSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMSrr\n");
            break;
        }
        case AArch64::FMINNMVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMVv4i32v\n");
            break;
        }
        case AArch64::FMINNMv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMv2f32\n");
            break;
        }
        case AArch64::FMINNMv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMv2f64\n");
            break;
        }
        case AArch64::FMINNMv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINNMv4f32\n");
            break;
        }
        case AArch64::FMINPv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINPv2f32\n");
            break;
        }
        case AArch64::FMINPv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINPv2f64\n");
            break;
        }
        case AArch64::FMINPv2i32p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINPv2i32p\n");
            break;
        }
        case AArch64::FMINPv2i64p: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINPv2i64p\n");
            break;
        }
        case AArch64::FMINPv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINPv4f32\n");
            break;
        }
        case AArch64::FMINSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINSrr\n");
            break;
        }
        case AArch64::FMINVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINVv4i32v\n");
            break;
        }
        case AArch64::FMINv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINv2f32\n");
            break;
        }
        case AArch64::FMINv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINv2f64\n");
            break;
        }
        case AArch64::FMINv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMINv4f32\n");
            break;
        }
        case AArch64::FMLAv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv1i32_indexed\n");
            break;
        }
        case AArch64::FMLAv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv1i64_indexed\n");
            break;
        }
        case AArch64::FMLAv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv2f32\n");
            break;
        }
        case AArch64::FMLAv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv2f64\n");
            break;
        }
        case AArch64::FMLAv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv2i32_indexed\n");
            break;
        }
        case AArch64::FMLAv2i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv2i64_indexed\n");
            break;
        }
        case AArch64::FMLAv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv4f32\n");
            break;
        }
        case AArch64::FMLAv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLAv4i32_indexed\n");
            break;
        }
        case AArch64::FMLSv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv1i32_indexed\n");
            break;
        }
        case AArch64::FMLSv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv1i64_indexed\n");
            break;
        }
        case AArch64::FMLSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv2f32\n");
            break;
        }
        case AArch64::FMLSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv2f64\n");
            break;
        }
        case AArch64::FMLSv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv2i32_indexed\n");
            break;
        }
        case AArch64::FMLSv2i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv2i64_indexed\n");
            break;
        }
        case AArch64::FMLSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv4f32\n");
            break;
        }
        case AArch64::FMLSv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMLSv4i32_indexed\n");
            break;
        }
        case AArch64::FMOVD0: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVD0\n");
            break;
        }
        case AArch64::FMOVDXHighr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVDXHighr\n");
            break;
        }
        case AArch64::FMOVDXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVDXr\n");
            break;
        }
        case AArch64::FMOVDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVDi\n");
            break;
        }
        case AArch64::FMOVDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVDr\n");
            break;
        }
        case AArch64::FMOVS0: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVS0\n");
            break;
        }
        case AArch64::FMOVSWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVSWr\n");
            break;
        }
        case AArch64::FMOVSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVSi\n");
            break;
        }
        case AArch64::FMOVSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVSr\n");
            break;
        }
        case AArch64::FMOVWSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVWSr\n");
            break;
        }
        case AArch64::FMOVXDHighr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVXDHighr\n");
            break;
        }
        case AArch64::FMOVXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVXDr\n");
            break;
        }
        case AArch64::FMOVv2f32_ns: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVv2f32_ns\n");
            break;
        }
        case AArch64::FMOVv2f64_ns: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVv2f64_ns\n");
            break;
        }
        case AArch64::FMOVv4f32_ns: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMOVv4f32_ns\n");
            break;
        }
        case AArch64::FMSUBDrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMSUBDrrr\n");
            break;
        }
        case AArch64::FMSUBSrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMSUBSrrr\n");
            break;
        }
        case AArch64::FMULDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULDrr\n");
            break;
        }
        case AArch64::FMULSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULSrr\n");
            break;
        }
        case AArch64::FMULX32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULX32\n");
            break;
        }
        case AArch64::FMULX64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULX64\n");
            break;
        }
        case AArch64::FMULXv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv1i32_indexed\n");
            break;
        }
        case AArch64::FMULXv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv1i64_indexed\n");
            break;
        }
        case AArch64::FMULXv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv2f32\n");
            break;
        }
        case AArch64::FMULXv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv2f64\n");
            break;
        }
        case AArch64::FMULXv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv2i32_indexed\n");
            break;
        }
        case AArch64::FMULXv2i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv2i64_indexed\n");
            break;
        }
        case AArch64::FMULXv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv4f32\n");
            break;
        }
        case AArch64::FMULXv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULXv4i32_indexed\n");
            break;
        }
        case AArch64::FMULv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv1i32_indexed\n");
            break;
        }
        case AArch64::FMULv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv1i64_indexed\n");
            break;
        }
        case AArch64::FMULv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv2f32\n");
            break;
        }
        case AArch64::FMULv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv2f64\n");
            break;
        }
        case AArch64::FMULv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv2i32_indexed\n");
            break;
        }
        case AArch64::FMULv2i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv2i64_indexed\n");
            break;
        }
        case AArch64::FMULv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv4f32\n");
            break;
        }
        case AArch64::FMULv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FMULv4i32_indexed\n");
            break;
        }
        case AArch64::FNEGDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNEGDr\n");
            break;
        }
        case AArch64::FNEGSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNEGSr\n");
            break;
        }
        case AArch64::FNEGv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNEGv2f32\n");
            break;
        }
        case AArch64::FNEGv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNEGv2f64\n");
            break;
        }
        case AArch64::FNEGv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNEGv4f32\n");
            break;
        }
        case AArch64::FNMADDDrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNMADDDrrr\n");
            break;
        }
        case AArch64::FNMADDSrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNMADDSrrr\n");
            break;
        }
        case AArch64::FNMSUBDrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNMSUBDrrr\n");
            break;
        }
        case AArch64::FNMSUBSrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNMSUBSrrr\n");
            break;
        }
        case AArch64::FNMULDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNMULDrr\n");
            break;
        }
        case AArch64::FNMULSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FNMULSrr\n");
            break;
        }
        case AArch64::FRECPEv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPEv1i32\n");
            break;
        }
        case AArch64::FRECPEv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPEv1i64\n");
            break;
        }
        case AArch64::FRECPEv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPEv2f32\n");
            break;
        }
        case AArch64::FRECPEv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPEv2f64\n");
            break;
        }
        case AArch64::FRECPEv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPEv4f32\n");
            break;
        }
        case AArch64::FRECPS32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPS32\n");
            break;
        }
        case AArch64::FRECPS64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPS64\n");
            break;
        }
        case AArch64::FRECPSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPSv2f32\n");
            break;
        }
        case AArch64::FRECPSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPSv2f64\n");
            break;
        }
        case AArch64::FRECPSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPSv4f32\n");
            break;
        }
        case AArch64::FRECPXv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPXv1i32\n");
            break;
        }
        case AArch64::FRECPXv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRECPXv1i64\n");
            break;
        }
        case AArch64::FRINTADr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTADr\n");
            break;
        }
        case AArch64::FRINTASr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTASr\n");
            break;
        }
        case AArch64::FRINTAv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTAv2f32\n");
            break;
        }
        case AArch64::FRINTAv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTAv2f64\n");
            break;
        }
        case AArch64::FRINTAv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTAv4f32\n");
            break;
        }
        case AArch64::FRINTIDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTIDr\n");
            break;
        }
        case AArch64::FRINTISr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTISr\n");
            break;
        }
        case AArch64::FRINTIv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTIv2f32\n");
            break;
        }
        case AArch64::FRINTIv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTIv2f64\n");
            break;
        }
        case AArch64::FRINTIv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTIv4f32\n");
            break;
        }
        case AArch64::FRINTMDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTMDr\n");
            break;
        }
        case AArch64::FRINTMSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTMSr\n");
            break;
        }
        case AArch64::FRINTMv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTMv2f32\n");
            break;
        }
        case AArch64::FRINTMv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTMv2f64\n");
            break;
        }
        case AArch64::FRINTMv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTMv4f32\n");
            break;
        }
        case AArch64::FRINTNDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTNDr\n");
            break;
        }
        case AArch64::FRINTNSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTNSr\n");
            break;
        }
        case AArch64::FRINTNv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTNv2f32\n");
            break;
        }
        case AArch64::FRINTNv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTNv2f64\n");
            break;
        }
        case AArch64::FRINTNv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTNv4f32\n");
            break;
        }
        case AArch64::FRINTPDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTPDr\n");
            break;
        }
        case AArch64::FRINTPSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTPSr\n");
            break;
        }
        case AArch64::FRINTPv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTPv2f32\n");
            break;
        }
        case AArch64::FRINTPv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTPv2f64\n");
            break;
        }
        case AArch64::FRINTPv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTPv4f32\n");
            break;
        }
        case AArch64::FRINTXDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTXDr\n");
            break;
        }
        case AArch64::FRINTXSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTXSr\n");
            break;
        }
        case AArch64::FRINTXv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTXv2f32\n");
            break;
        }
        case AArch64::FRINTXv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTXv2f64\n");
            break;
        }
        case AArch64::FRINTXv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTXv4f32\n");
            break;
        }
        case AArch64::FRINTZDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTZDr\n");
            break;
        }
        case AArch64::FRINTZSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTZSr\n");
            break;
        }
        case AArch64::FRINTZv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTZv2f32\n");
            break;
        }
        case AArch64::FRINTZv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTZv2f64\n");
            break;
        }
        case AArch64::FRINTZv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRINTZv4f32\n");
            break;
        }
        case AArch64::FRSQRTEv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTEv1i32\n");
            break;
        }
        case AArch64::FRSQRTEv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTEv1i64\n");
            break;
        }
        case AArch64::FRSQRTEv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTEv2f32\n");
            break;
        }
        case AArch64::FRSQRTEv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTEv2f64\n");
            break;
        }
        case AArch64::FRSQRTEv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTEv4f32\n");
            break;
        }
        case AArch64::FRSQRTS32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTS32\n");
            break;
        }
        case AArch64::FRSQRTS64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTS64\n");
            break;
        }
        case AArch64::FRSQRTSv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTSv2f32\n");
            break;
        }
        case AArch64::FRSQRTSv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTSv2f64\n");
            break;
        }
        case AArch64::FRSQRTSv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FRSQRTSv4f32\n");
            break;
        }
        case AArch64::FSQRTDr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSQRTDr\n");
            break;
        }
        case AArch64::FSQRTSr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSQRTSr\n");
            break;
        }
        case AArch64::FSQRTv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSQRTv2f32\n");
            break;
        }
        case AArch64::FSQRTv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSQRTv2f64\n");
            break;
        }
        case AArch64::FSQRTv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSQRTv4f32\n");
            break;
        }
        case AArch64::FSUBDrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSUBDrr\n");
            break;
        }
        case AArch64::FSUBSrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSUBSrr\n");
            break;
        }
        case AArch64::FSUBv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSUBv2f32\n");
            break;
        }
        case AArch64::FSUBv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSUBv2f64\n");
            break;
        }
        case AArch64::FSUBv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " FSUBv4f32\n");
            break;
        }
        case AArch64::HINT: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " HINT\n");
            break;
        }
        case AArch64::HLT: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " HLT\n");
            break;
        }
        case AArch64::HVC: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " HVC\n");
            break;
        }
        case AArch64::INSvi16gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi16gpr\n");
            break;
        }
        case AArch64::INSvi16lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi16lane\n");
            break;
        }
        case AArch64::INSvi32gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi32gpr\n");
            break;
        }
        case AArch64::INSvi32lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi32lane\n");
            break;
        }
        case AArch64::INSvi64gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi64gpr\n");
            break;
        }
        case AArch64::INSvi64lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi64lane\n");
            break;
        }
        case AArch64::INSvi8gpr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi8gpr\n");
            break;
        }
        case AArch64::INSvi8lane: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " INSvi8lane\n");
            break;
        }
        case AArch64::ISB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ISB\n");
            break;
        }
        case AArch64::LD1Fourv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv16b\n");
            break;
        }
        case AArch64::LD1Fourv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv16b_POST\n");
            break;
        }
        case AArch64::LD1Fourv1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv1d\n");
            break;
        }
        case AArch64::LD1Fourv1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv1d_POST\n");
            break;
        }
        case AArch64::LD1Fourv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv2d\n");
            break;
        }
        case AArch64::LD1Fourv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv2d_POST\n");
            break;
        }
        case AArch64::LD1Fourv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv2s\n");
            break;
        }
        case AArch64::LD1Fourv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv2s_POST\n");
            break;
        }
        case AArch64::LD1Fourv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv4h\n");
            break;
        }
        case AArch64::LD1Fourv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv4h_POST\n");
            break;
        }
        case AArch64::LD1Fourv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv4s\n");
            break;
        }
        case AArch64::LD1Fourv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv4s_POST\n");
            break;
        }
        case AArch64::LD1Fourv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv8b\n");
            break;
        }
        case AArch64::LD1Fourv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv8b_POST\n");
            break;
        }
        case AArch64::LD1Fourv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv8h\n");
            break;
        }
        case AArch64::LD1Fourv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Fourv8h_POST\n");
            break;
        }
        case AArch64::LD1Onev16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev16b\n");
            break;
        }
        case AArch64::LD1Onev16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev16b_POST\n");
            break;
        }
        case AArch64::LD1Onev1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev1d\n");
            break;
        }
        case AArch64::LD1Onev1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev1d_POST\n");
            break;
        }
        case AArch64::LD1Onev2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev2d\n");
            break;
        }
        case AArch64::LD1Onev2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev2d_POST\n");
            break;
        }
        case AArch64::LD1Onev2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev2s\n");
            break;
        }
        case AArch64::LD1Onev2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev2s_POST\n");
            break;
        }
        case AArch64::LD1Onev4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev4h\n");
            break;
        }
        case AArch64::LD1Onev4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev4h_POST\n");
            break;
        }
        case AArch64::LD1Onev4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev4s\n");
            break;
        }
        case AArch64::LD1Onev4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev4s_POST\n");
            break;
        }
        case AArch64::LD1Onev8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev8b\n");
            break;
        }
        case AArch64::LD1Onev8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev8b_POST\n");
            break;
        }
        case AArch64::LD1Onev8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev8h\n");
            break;
        }
        case AArch64::LD1Onev8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Onev8h_POST\n");
            break;
        }
        case AArch64::LD1Rv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv16b\n");
            break;
        }
        case AArch64::LD1Rv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv16b_POST\n");
            break;
        }
        case AArch64::LD1Rv1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv1d\n");
            break;
        }
        case AArch64::LD1Rv1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv1d_POST\n");
            break;
        }
        case AArch64::LD1Rv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv2d\n");
            break;
        }
        case AArch64::LD1Rv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv2d_POST\n");
            break;
        }
        case AArch64::LD1Rv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv2s\n");
            break;
        }
        case AArch64::LD1Rv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv2s_POST\n");
            break;
        }
        case AArch64::LD1Rv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv4h\n");
            break;
        }
        case AArch64::LD1Rv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv4h_POST\n");
            break;
        }
        case AArch64::LD1Rv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv4s\n");
            break;
        }
        case AArch64::LD1Rv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv4s_POST\n");
            break;
        }
        case AArch64::LD1Rv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv8b\n");
            break;
        }
        case AArch64::LD1Rv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv8b_POST\n");
            break;
        }
        case AArch64::LD1Rv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv8h\n");
            break;
        }
        case AArch64::LD1Rv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Rv8h_POST\n");
            break;
        }
        case AArch64::LD1Threev16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev16b\n");
            break;
        }
        case AArch64::LD1Threev16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev16b_POST\n");
            break;
        }
        case AArch64::LD1Threev1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev1d\n");
            break;
        }
        case AArch64::LD1Threev1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev1d_POST\n");
            break;
        }
        case AArch64::LD1Threev2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev2d\n");
            break;
        }
        case AArch64::LD1Threev2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev2d_POST\n");
            break;
        }
        case AArch64::LD1Threev2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev2s\n");
            break;
        }
        case AArch64::LD1Threev2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev2s_POST\n");
            break;
        }
        case AArch64::LD1Threev4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev4h\n");
            break;
        }
        case AArch64::LD1Threev4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev4h_POST\n");
            break;
        }
        case AArch64::LD1Threev4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev4s\n");
            break;
        }
        case AArch64::LD1Threev4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev4s_POST\n");
            break;
        }
        case AArch64::LD1Threev8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev8b\n");
            break;
        }
        case AArch64::LD1Threev8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev8b_POST\n");
            break;
        }
        case AArch64::LD1Threev8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev8h\n");
            break;
        }
        case AArch64::LD1Threev8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Threev8h_POST\n");
            break;
        }
        case AArch64::LD1Twov16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov16b\n");
            break;
        }
        case AArch64::LD1Twov16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov16b_POST\n");
            break;
        }
        case AArch64::LD1Twov1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov1d\n");
            break;
        }
        case AArch64::LD1Twov1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov1d_POST\n");
            break;
        }
        case AArch64::LD1Twov2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov2d\n");
            break;
        }
        case AArch64::LD1Twov2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov2d_POST\n");
            break;
        }
        case AArch64::LD1Twov2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov2s\n");
            break;
        }
        case AArch64::LD1Twov2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov2s_POST\n");
            break;
        }
        case AArch64::LD1Twov4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov4h\n");
            break;
        }
        case AArch64::LD1Twov4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov4h_POST\n");
            break;
        }
        case AArch64::LD1Twov4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov4s\n");
            break;
        }
        case AArch64::LD1Twov4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov4s_POST\n");
            break;
        }
        case AArch64::LD1Twov8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov8b\n");
            break;
        }
        case AArch64::LD1Twov8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov8b_POST\n");
            break;
        }
        case AArch64::LD1Twov8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov8h\n");
            break;
        }
        case AArch64::LD1Twov8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1Twov8h_POST\n");
            break;
        }
        case AArch64::LD1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i16\n");
            break;
        }
        case AArch64::LD1i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i16_POST\n");
            break;
        }
        case AArch64::LD1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i32\n");
            break;
        }
        case AArch64::LD1i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i32_POST\n");
            break;
        }
        case AArch64::LD1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i64\n");
            break;
        }
        case AArch64::LD1i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i64_POST\n");
            break;
        }
        case AArch64::LD1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i8\n");
            break;
        }
        case AArch64::LD1i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD1i8_POST\n");
            break;
        }
        case AArch64::LD2Rv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv16b\n");
            break;
        }
        case AArch64::LD2Rv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv16b_POST\n");
            break;
        }
        case AArch64::LD2Rv1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv1d\n");
            break;
        }
        case AArch64::LD2Rv1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv1d_POST\n");
            break;
        }
        case AArch64::LD2Rv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv2d\n");
            break;
        }
        case AArch64::LD2Rv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv2d_POST\n");
            break;
        }
        case AArch64::LD2Rv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv2s\n");
            break;
        }
        case AArch64::LD2Rv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv2s_POST\n");
            break;
        }
        case AArch64::LD2Rv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv4h\n");
            break;
        }
        case AArch64::LD2Rv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv4h_POST\n");
            break;
        }
        case AArch64::LD2Rv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv4s\n");
            break;
        }
        case AArch64::LD2Rv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv4s_POST\n");
            break;
        }
        case AArch64::LD2Rv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv8b\n");
            break;
        }
        case AArch64::LD2Rv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv8b_POST\n");
            break;
        }
        case AArch64::LD2Rv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv8h\n");
            break;
        }
        case AArch64::LD2Rv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Rv8h_POST\n");
            break;
        }
        case AArch64::LD2Twov16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov16b\n");
            break;
        }
        case AArch64::LD2Twov16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov16b_POST\n");
            break;
        }
        case AArch64::LD2Twov2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov2d\n");
            break;
        }
        case AArch64::LD2Twov2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov2d_POST\n");
            break;
        }
        case AArch64::LD2Twov2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov2s\n");
            break;
        }
        case AArch64::LD2Twov2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov2s_POST\n");
            break;
        }
        case AArch64::LD2Twov4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov4h\n");
            break;
        }
        case AArch64::LD2Twov4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov4h_POST\n");
            break;
        }
        case AArch64::LD2Twov4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov4s\n");
            break;
        }
        case AArch64::LD2Twov4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov4s_POST\n");
            break;
        }
        case AArch64::LD2Twov8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov8b\n");
            break;
        }
        case AArch64::LD2Twov8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov8b_POST\n");
            break;
        }
        case AArch64::LD2Twov8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov8h\n");
            break;
        }
        case AArch64::LD2Twov8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2Twov8h_POST\n");
            break;
        }
        case AArch64::LD2i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i16\n");
            break;
        }
        case AArch64::LD2i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i16_POST\n");
            break;
        }
        case AArch64::LD2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i32\n");
            break;
        }
        case AArch64::LD2i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i32_POST\n");
            break;
        }
        case AArch64::LD2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i64\n");
            break;
        }
        case AArch64::LD2i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i64_POST\n");
            break;
        }
        case AArch64::LD2i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i8\n");
            break;
        }
        case AArch64::LD2i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD2i8_POST\n");
            break;
        }
        case AArch64::LD3Rv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv16b\n");
            break;
        }
        case AArch64::LD3Rv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv16b_POST\n");
            break;
        }
        case AArch64::LD3Rv1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv1d\n");
            break;
        }
        case AArch64::LD3Rv1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv1d_POST\n");
            break;
        }
        case AArch64::LD3Rv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv2d\n");
            break;
        }
        case AArch64::LD3Rv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv2d_POST\n");
            break;
        }
        case AArch64::LD3Rv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv2s\n");
            break;
        }
        case AArch64::LD3Rv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv2s_POST\n");
            break;
        }
        case AArch64::LD3Rv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv4h\n");
            break;
        }
        case AArch64::LD3Rv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv4h_POST\n");
            break;
        }
        case AArch64::LD3Rv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv4s\n");
            break;
        }
        case AArch64::LD3Rv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv4s_POST\n");
            break;
        }
        case AArch64::LD3Rv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv8b\n");
            break;
        }
        case AArch64::LD3Rv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv8b_POST\n");
            break;
        }
        case AArch64::LD3Rv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv8h\n");
            break;
        }
        case AArch64::LD3Rv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Rv8h_POST\n");
            break;
        }
        case AArch64::LD3Threev16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev16b\n");
            break;
        }
        case AArch64::LD3Threev16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev16b_POST\n");
            break;
        }
        case AArch64::LD3Threev2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev2d\n");
            break;
        }
        case AArch64::LD3Threev2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev2d_POST\n");
            break;
        }
        case AArch64::LD3Threev2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev2s\n");
            break;
        }
        case AArch64::LD3Threev2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev2s_POST\n");
            break;
        }
        case AArch64::LD3Threev4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev4h\n");
            break;
        }
        case AArch64::LD3Threev4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev4h_POST\n");
            break;
        }
        case AArch64::LD3Threev4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev4s\n");
            break;
        }
        case AArch64::LD3Threev4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev4s_POST\n");
            break;
        }
        case AArch64::LD3Threev8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev8b\n");
            break;
        }
        case AArch64::LD3Threev8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev8b_POST\n");
            break;
        }
        case AArch64::LD3Threev8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev8h\n");
            break;
        }
        case AArch64::LD3Threev8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3Threev8h_POST\n");
            break;
        }
        case AArch64::LD3i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i16\n");
            break;
        }
        case AArch64::LD3i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i16_POST\n");
            break;
        }
        case AArch64::LD3i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i32\n");
            break;
        }
        case AArch64::LD3i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i32_POST\n");
            break;
        }
        case AArch64::LD3i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i64\n");
            break;
        }
        case AArch64::LD3i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i64_POST\n");
            break;
        }
        case AArch64::LD3i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i8\n");
            break;
        }
        case AArch64::LD3i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD3i8_POST\n");
            break;
        }
        case AArch64::LD4Fourv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv16b\n");
            break;
        }
        case AArch64::LD4Fourv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv16b_POST\n");
            break;
        }
        case AArch64::LD4Fourv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv2d\n");
            break;
        }
        case AArch64::LD4Fourv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv2d_POST\n");
            break;
        }
        case AArch64::LD4Fourv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv2s\n");
            break;
        }
        case AArch64::LD4Fourv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv2s_POST\n");
            break;
        }
        case AArch64::LD4Fourv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv4h\n");
            break;
        }
        case AArch64::LD4Fourv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv4h_POST\n");
            break;
        }
        case AArch64::LD4Fourv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv4s\n");
            break;
        }
        case AArch64::LD4Fourv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv4s_POST\n");
            break;
        }
        case AArch64::LD4Fourv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv8b\n");
            break;
        }
        case AArch64::LD4Fourv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv8b_POST\n");
            break;
        }
        case AArch64::LD4Fourv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv8h\n");
            break;
        }
        case AArch64::LD4Fourv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Fourv8h_POST\n");
            break;
        }
        case AArch64::LD4Rv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv16b\n");
            break;
        }
        case AArch64::LD4Rv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv16b_POST\n");
            break;
        }
        case AArch64::LD4Rv1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv1d\n");
            break;
        }
        case AArch64::LD4Rv1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv1d_POST\n");
            break;
        }
        case AArch64::LD4Rv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv2d\n");
            break;
        }
        case AArch64::LD4Rv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv2d_POST\n");
            break;
        }
        case AArch64::LD4Rv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv2s\n");
            break;
        }
        case AArch64::LD4Rv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv2s_POST\n");
            break;
        }
        case AArch64::LD4Rv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv4h\n");
            break;
        }
        case AArch64::LD4Rv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv4h_POST\n");
            break;
        }
        case AArch64::LD4Rv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv4s\n");
            break;
        }
        case AArch64::LD4Rv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv4s_POST\n");
            break;
        }
        case AArch64::LD4Rv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv8b\n");
            break;
        }
        case AArch64::LD4Rv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv8b_POST\n");
            break;
        }
        case AArch64::LD4Rv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv8h\n");
            break;
        }
        case AArch64::LD4Rv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4Rv8h_POST\n");
            break;
        }
        case AArch64::LD4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i16\n");
            break;
        }
        case AArch64::LD4i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i16_POST\n");
            break;
        }
        case AArch64::LD4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i32\n");
            break;
        }
        case AArch64::LD4i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i32_POST\n");
            break;
        }
        case AArch64::LD4i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i64\n");
            break;
        }
        case AArch64::LD4i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i64_POST\n");
            break;
        }
        case AArch64::LD4i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i8\n");
            break;
        }
        case AArch64::LD4i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LD4i8_POST\n");
            break;
        }
        case AArch64::LDADDALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDALb\n");
            break;
        }
        case AArch64::LDADDALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDALd\n");
            break;
        }
        case AArch64::LDADDALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDALh\n");
            break;
        }
        case AArch64::LDADDALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDALs\n");
            break;
        }
        case AArch64::LDADDAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDAb\n");
            break;
        }
        case AArch64::LDADDAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDAd\n");
            break;
        }
        case AArch64::LDADDAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDAh\n");
            break;
        }
        case AArch64::LDADDAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDAs\n");
            break;
        }
        case AArch64::LDADDLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDLb\n");
            break;
        }
        case AArch64::LDADDLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDLd\n");
            break;
        }
        case AArch64::LDADDLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDLh\n");
            break;
        }
        case AArch64::LDADDLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDLs\n");
            break;
        }
        case AArch64::LDADDb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDb\n");
            break;
        }
        case AArch64::LDADDd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDd\n");
            break;
        }
        case AArch64::LDADDh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDh\n");
            break;
        }
        case AArch64::LDADDs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDADDs\n");
            break;
        }
        case AArch64::LDARB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDARB\n");
            break;
        }
        case AArch64::LDARH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDARH\n");
            break;
        }
        case AArch64::LDARW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDARW\n");
            break;
        }
        case AArch64::LDARX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDARX\n");
            break;
        }
        case AArch64::LDAXPW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDAXPW\n");
            break;
        }
        case AArch64::LDAXPX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDAXPX\n");
            break;
        }
        case AArch64::LDAXRB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDAXRB\n");
            break;
        }
        case AArch64::LDAXRH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDAXRH\n");
            break;
        }
        case AArch64::LDAXRW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDAXRW\n");
            break;
        }
        case AArch64::LDAXRX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDAXRX\n");
            break;
        }
        case AArch64::LDCLRALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRALb\n");
            break;
        }
        case AArch64::LDCLRALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRALd\n");
            break;
        }
        case AArch64::LDCLRALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRALh\n");
            break;
        }
        case AArch64::LDCLRALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRALs\n");
            break;
        }
        case AArch64::LDCLRAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRAb\n");
            break;
        }
        case AArch64::LDCLRAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRAd\n");
            break;
        }
        case AArch64::LDCLRAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRAh\n");
            break;
        }
        case AArch64::LDCLRAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRAs\n");
            break;
        }
        case AArch64::LDCLRLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRLb\n");
            break;
        }
        case AArch64::LDCLRLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRLd\n");
            break;
        }
        case AArch64::LDCLRLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRLh\n");
            break;
        }
        case AArch64::LDCLRLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRLs\n");
            break;
        }
        case AArch64::LDCLRb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRb\n");
            break;
        }
        case AArch64::LDCLRd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRd\n");
            break;
        }
        case AArch64::LDCLRh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRh\n");
            break;
        }
        case AArch64::LDCLRs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDCLRs\n");
            break;
        }
        case AArch64::LDEORALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORALb\n");
            break;
        }
        case AArch64::LDEORALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORALd\n");
            break;
        }
        case AArch64::LDEORALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORALh\n");
            break;
        }
        case AArch64::LDEORALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORALs\n");
            break;
        }
        case AArch64::LDEORAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORAb\n");
            break;
        }
        case AArch64::LDEORAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORAd\n");
            break;
        }
        case AArch64::LDEORAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORAh\n");
            break;
        }
        case AArch64::LDEORAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORAs\n");
            break;
        }
        case AArch64::LDEORLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORLb\n");
            break;
        }
        case AArch64::LDEORLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORLd\n");
            break;
        }
        case AArch64::LDEORLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORLh\n");
            break;
        }
        case AArch64::LDEORLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORLs\n");
            break;
        }
        case AArch64::LDEORb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORb\n");
            break;
        }
        case AArch64::LDEORd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORd\n");
            break;
        }
        case AArch64::LDEORh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORh\n");
            break;
        }
        case AArch64::LDEORs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDEORs\n");
            break;
        }
        case AArch64::LDLARB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDLARB\n");
            break;
        }
        case AArch64::LDLARH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDLARH\n");
            break;
        }
        case AArch64::LDLARW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDLARW\n");
            break;
        }
        case AArch64::LDLARX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDLARX\n");
            break;
        }
        case AArch64::LDNPDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDNPDi\n");
            break;
        }
        case AArch64::LDNPQi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDNPQi\n");
            break;
        }
        case AArch64::LDNPSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDNPSi\n");
            break;
        }
        case AArch64::LDNPWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDNPWi\n");
            break;
        }
        case AArch64::LDNPXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDNPXi\n");
            break;
        }
        case AArch64::LDPDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPDi\n");
            break;
        }
        case AArch64::LDPDpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPDpost\n");
            break;
        }
        case AArch64::LDPDpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPDpre\n");
            break;
        }
        case AArch64::LDPQi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPQi\n");
            break;
        }
        case AArch64::LDPQpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPQpost\n");
            break;
        }
        case AArch64::LDPQpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPQpre\n");
            break;
        }
        case AArch64::LDPSWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPSWi\n");
            break;
        }
        case AArch64::LDPSWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPSWpost\n");
            break;
        }
        case AArch64::LDPSWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPSWpre\n");
            break;
        }
        case AArch64::LDPSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPSi\n");
            break;
        }
        case AArch64::LDPSpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPSpost\n");
            break;
        }
        case AArch64::LDPSpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPSpre\n");
            break;
        }
        case AArch64::LDPWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPWi\n");
            break;
        }
        case AArch64::LDPWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPWpost\n");
            break;
        }
        case AArch64::LDPWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPWpre\n");
            break;
        }
        case AArch64::LDPXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPXi\n");
            break;
        }
        case AArch64::LDPXpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPXpost\n");
            break;
        }
        case AArch64::LDPXpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDPXpre\n");
            break;
        }
        case AArch64::LDRBBpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBBpost\n");
            break;
        }
        case AArch64::LDRBBpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBBpre\n");
            break;
        }
        case AArch64::LDRBBroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBBroW\n");
            break;
        }
        case AArch64::LDRBBroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBBroX\n");
            break;
        }
        case AArch64::LDRBBui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBBui\n");
            break;
        }
        case AArch64::LDRBpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBpost\n");
            break;
        }
        case AArch64::LDRBpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBpre\n");
            break;
        }
        case AArch64::LDRBroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBroW\n");
            break;
        }
        case AArch64::LDRBroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBroX\n");
            break;
        }
        case AArch64::LDRBui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRBui\n");
            break;
        }
        case AArch64::LDRDl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRDl\n");
            break;
        }
        case AArch64::LDRDpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRDpost\n");
            break;
        }
        case AArch64::LDRDpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRDpre\n");
            break;
        }
        case AArch64::LDRDroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRDroW\n");
            break;
        }
        case AArch64::LDRDroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRDroX\n");
            break;
        }
        case AArch64::LDRDui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRDui\n");
            break;
        }
        case AArch64::LDRHHpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHHpost\n");
            break;
        }
        case AArch64::LDRHHpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHHpre\n");
            break;
        }
        case AArch64::LDRHHroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHHroW\n");
            break;
        }
        case AArch64::LDRHHroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHHroX\n");
            break;
        }
        case AArch64::LDRHHui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHHui\n");
            break;
        }
        case AArch64::LDRHpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHpost\n");
            break;
        }
        case AArch64::LDRHpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHpre\n");
            break;
        }
        case AArch64::LDRHroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHroW\n");
            break;
        }
        case AArch64::LDRHroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHroX\n");
            break;
        }
        case AArch64::LDRHui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRHui\n");
            break;
        }
        case AArch64::LDRQl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRQl\n");
            break;
        }
        case AArch64::LDRQpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRQpost\n");
            break;
        }
        case AArch64::LDRQpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRQpre\n");
            break;
        }
        case AArch64::LDRQroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRQroW\n");
            break;
        }
        case AArch64::LDRQroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRQroX\n");
            break;
        }
        case AArch64::LDRQui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRQui\n");
            break;
        }
        case AArch64::LDRSBWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBWpost\n");
            break;
        }
        case AArch64::LDRSBWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBWpre\n");
            break;
        }
        case AArch64::LDRSBWroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBWroW\n");
            break;
        }
        case AArch64::LDRSBWroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBWroX\n");
            break;
        }
        case AArch64::LDRSBWui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBWui\n");
            break;
        }
        case AArch64::LDRSBXpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBXpost\n");
            break;
        }
        case AArch64::LDRSBXpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBXpre\n");
            break;
        }
        case AArch64::LDRSBXroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBXroW\n");
            break;
        }
        case AArch64::LDRSBXroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBXroX\n");
            break;
        }
        case AArch64::LDRSBXui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSBXui\n");
            break;
        }
        case AArch64::LDRSHWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHWpost\n");
            break;
        }
        case AArch64::LDRSHWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHWpre\n");
            break;
        }
        case AArch64::LDRSHWroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHWroW\n");
            break;
        }
        case AArch64::LDRSHWroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHWroX\n");
            break;
        }
        case AArch64::LDRSHWui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHWui\n");
            break;
        }
        case AArch64::LDRSHXpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHXpost\n");
            break;
        }
        case AArch64::LDRSHXpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHXpre\n");
            break;
        }
        case AArch64::LDRSHXroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHXroW\n");
            break;
        }
        case AArch64::LDRSHXroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHXroX\n");
            break;
        }
        case AArch64::LDRSHXui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSHXui\n");
            break;
        }
        case AArch64::LDRSWl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSWl\n");
            break;
        }
        case AArch64::LDRSWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSWpost\n");
            break;
        }
        case AArch64::LDRSWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSWpre\n");
            break;
        }
        case AArch64::LDRSWroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSWroW\n");
            break;
        }
        case AArch64::LDRSWroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSWroX\n");
            break;
        }
        case AArch64::LDRSWui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSWui\n");
            break;
        }
        case AArch64::LDRSl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSl\n");
            break;
        }
        case AArch64::LDRSpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSpost\n");
            break;
        }
        case AArch64::LDRSpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSpre\n");
            break;
        }
        case AArch64::LDRSroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSroW\n");
            break;
        }
        case AArch64::LDRSroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSroX\n");
            break;
        }
        case AArch64::LDRSui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRSui\n");
            break;
        }
        case AArch64::LDRWl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRWl\n");
            break;
        }
        case AArch64::LDRWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRWpost\n");
            break;
        }
        case AArch64::LDRWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRWpre\n");
            break;
        }
        case AArch64::LDRWroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRWroW\n");
            break;
        }
        case AArch64::LDRWroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRWroX\n");
            break;
        }
        case AArch64::LDRWui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRWui\n");
            break;
        }
        case AArch64::LDRXl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRXl\n");
            break;
        }
        case AArch64::LDRXpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRXpost\n");
            break;
        }
        case AArch64::LDRXpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRXpre\n");
            break;
        }
        case AArch64::LDRXroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRXroW\n");
            break;
        }
        case AArch64::LDRXroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRXroX\n");
            break;
        }
        case AArch64::LDRXui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDRXui\n");
            break;
        }
        case AArch64::LDSETALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETALb\n");
            break;
        }
        case AArch64::LDSETALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETALd\n");
            break;
        }
        case AArch64::LDSETALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETALh\n");
            break;
        }
        case AArch64::LDSETALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETALs\n");
            break;
        }
        case AArch64::LDSETAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETAb\n");
            break;
        }
        case AArch64::LDSETAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETAd\n");
            break;
        }
        case AArch64::LDSETAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETAh\n");
            break;
        }
        case AArch64::LDSETAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETAs\n");
            break;
        }
        case AArch64::LDSETLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETLb\n");
            break;
        }
        case AArch64::LDSETLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETLd\n");
            break;
        }
        case AArch64::LDSETLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETLh\n");
            break;
        }
        case AArch64::LDSETLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETLs\n");
            break;
        }
        case AArch64::LDSETb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETb\n");
            break;
        }
        case AArch64::LDSETd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETd\n");
            break;
        }
        case AArch64::LDSETh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETh\n");
            break;
        }
        case AArch64::LDSETs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSETs\n");
            break;
        }
        case AArch64::LDSMAXALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXALb\n");
            break;
        }
        case AArch64::LDSMAXALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXALd\n");
            break;
        }
        case AArch64::LDSMAXALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXALh\n");
            break;
        }
        case AArch64::LDSMAXALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXALs\n");
            break;
        }
        case AArch64::LDSMAXAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXAb\n");
            break;
        }
        case AArch64::LDSMAXAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXAd\n");
            break;
        }
        case AArch64::LDSMAXAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXAh\n");
            break;
        }
        case AArch64::LDSMAXAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXAs\n");
            break;
        }
        case AArch64::LDSMAXLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXLb\n");
            break;
        }
        case AArch64::LDSMAXLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXLd\n");
            break;
        }
        case AArch64::LDSMAXLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXLh\n");
            break;
        }
        case AArch64::LDSMAXLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXLs\n");
            break;
        }
        case AArch64::LDSMAXb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXb\n");
            break;
        }
        case AArch64::LDSMAXd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXd\n");
            break;
        }
        case AArch64::LDSMAXh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXh\n");
            break;
        }
        case AArch64::LDSMAXs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMAXs\n");
            break;
        }
        case AArch64::LDSMINALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINALb\n");
            break;
        }
        case AArch64::LDSMINALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINALd\n");
            break;
        }
        case AArch64::LDSMINALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINALh\n");
            break;
        }
        case AArch64::LDSMINALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINALs\n");
            break;
        }
        case AArch64::LDSMINAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINAb\n");
            break;
        }
        case AArch64::LDSMINAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINAd\n");
            break;
        }
        case AArch64::LDSMINAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINAh\n");
            break;
        }
        case AArch64::LDSMINAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINAs\n");
            break;
        }
        case AArch64::LDSMINLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINLb\n");
            break;
        }
        case AArch64::LDSMINLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINLd\n");
            break;
        }
        case AArch64::LDSMINLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINLh\n");
            break;
        }
        case AArch64::LDSMINLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINLs\n");
            break;
        }
        case AArch64::LDSMINb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINb\n");
            break;
        }
        case AArch64::LDSMINd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINd\n");
            break;
        }
        case AArch64::LDSMINh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINh\n");
            break;
        }
        case AArch64::LDSMINs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDSMINs\n");
            break;
        }
        case AArch64::LDTRBi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRBi\n");
            break;
        }
        case AArch64::LDTRHi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRHi\n");
            break;
        }
        case AArch64::LDTRSBWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRSBWi\n");
            break;
        }
        case AArch64::LDTRSBXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRSBXi\n");
            break;
        }
        case AArch64::LDTRSHWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRSHWi\n");
            break;
        }
        case AArch64::LDTRSHXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRSHXi\n");
            break;
        }
        case AArch64::LDTRSWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRSWi\n");
            break;
        }
        case AArch64::LDTRWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRWi\n");
            break;
        }
        case AArch64::LDTRXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDTRXi\n");
            break;
        }
        case AArch64::LDUMAXALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXALb\n");
            break;
        }
        case AArch64::LDUMAXALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXALd\n");
            break;
        }
        case AArch64::LDUMAXALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXALh\n");
            break;
        }
        case AArch64::LDUMAXALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXALs\n");
            break;
        }
        case AArch64::LDUMAXAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXAb\n");
            break;
        }
        case AArch64::LDUMAXAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXAd\n");
            break;
        }
        case AArch64::LDUMAXAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXAh\n");
            break;
        }
        case AArch64::LDUMAXAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXAs\n");
            break;
        }
        case AArch64::LDUMAXLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXLb\n");
            break;
        }
        case AArch64::LDUMAXLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXLd\n");
            break;
        }
        case AArch64::LDUMAXLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXLh\n");
            break;
        }
        case AArch64::LDUMAXLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXLs\n");
            break;
        }
        case AArch64::LDUMAXb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXb\n");
            break;
        }
        case AArch64::LDUMAXd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXd\n");
            break;
        }
        case AArch64::LDUMAXh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXh\n");
            break;
        }
        case AArch64::LDUMAXs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMAXs\n");
            break;
        }
        case AArch64::LDUMINALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINALb\n");
            break;
        }
        case AArch64::LDUMINALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINALd\n");
            break;
        }
        case AArch64::LDUMINALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINALh\n");
            break;
        }
        case AArch64::LDUMINALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINALs\n");
            break;
        }
        case AArch64::LDUMINAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINAb\n");
            break;
        }
        case AArch64::LDUMINAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINAd\n");
            break;
        }
        case AArch64::LDUMINAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINAh\n");
            break;
        }
        case AArch64::LDUMINAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINAs\n");
            break;
        }
        case AArch64::LDUMINLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINLb\n");
            break;
        }
        case AArch64::LDUMINLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINLd\n");
            break;
        }
        case AArch64::LDUMINLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINLh\n");
            break;
        }
        case AArch64::LDUMINLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINLs\n");
            break;
        }
        case AArch64::LDUMINb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINb\n");
            break;
        }
        case AArch64::LDUMINd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINd\n");
            break;
        }
        case AArch64::LDUMINh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINh\n");
            break;
        }
        case AArch64::LDUMINs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDUMINs\n");
            break;
        }
        case AArch64::LDURBBi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURBBi\n");
            break;
        }
        case AArch64::LDURBi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURBi\n");
            break;
        }
        case AArch64::LDURDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURDi\n");
            break;
        }
        case AArch64::LDURHHi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURHHi\n");
            break;
        }
        case AArch64::LDURHi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURHi\n");
            break;
        }
        case AArch64::LDURQi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURQi\n");
            break;
        }
        case AArch64::LDURSBWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURSBWi\n");
            break;
        }
        case AArch64::LDURSBXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURSBXi\n");
            break;
        }
        case AArch64::LDURSHWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURSHWi\n");
            break;
        }
        case AArch64::LDURSHXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURSHXi\n");
            break;
        }
        case AArch64::LDURSWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURSWi\n");
            break;
        }
        case AArch64::LDURSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURSi\n");
            break;
        }
        case AArch64::LDURWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURWi\n");
            break;
        }
        case AArch64::LDURXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDURXi\n");
            break;
        }
        case AArch64::LDXPW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDXPW\n");
            break;
        }
        case AArch64::LDXPX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDXPX\n");
            break;
        }
        case AArch64::LDXRB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDXRB\n");
            break;
        }
        case AArch64::LDXRH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDXRH\n");
            break;
        }
        case AArch64::LDXRW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDXRW\n");
            break;
        }
        case AArch64::LDXRX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LDXRX\n");
            break;
        }
        case AArch64::LOADgot: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LOADgot\n");
            break;
        }
        case AArch64::LSLVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LSLVWr\n");
            break;
        }
        case AArch64::LSLVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LSLVXr\n");
            break;
        }
        case AArch64::LSRVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LSRVWr\n");
            break;
        }
        case AArch64::LSRVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " LSRVXr\n");
            break;
        }
        case AArch64::MADDWrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MADDWrrr\n");
            break;
        }
        case AArch64::MADDXrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MADDXrrr\n");
            break;
        }
        case AArch64::MLAv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv16i8\n");
            break;
        }
        case AArch64::MLAv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv2i32\n");
            break;
        }
        case AArch64::MLAv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv2i32_indexed\n");
            break;
        }
        case AArch64::MLAv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv4i16\n");
            break;
        }
        case AArch64::MLAv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv4i16_indexed\n");
            break;
        }
        case AArch64::MLAv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv4i32\n");
            break;
        }
        case AArch64::MLAv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv4i32_indexed\n");
            break;
        }
        case AArch64::MLAv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv8i16\n");
            break;
        }
        case AArch64::MLAv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv8i16_indexed\n");
            break;
        }
        case AArch64::MLAv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLAv8i8\n");
            break;
        }
        case AArch64::MLSv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv16i8\n");
            break;
        }
        case AArch64::MLSv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv2i32\n");
            break;
        }
        case AArch64::MLSv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv2i32_indexed\n");
            break;
        }
        case AArch64::MLSv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv4i16\n");
            break;
        }
        case AArch64::MLSv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv4i16_indexed\n");
            break;
        }
        case AArch64::MLSv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv4i32\n");
            break;
        }
        case AArch64::MLSv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv4i32_indexed\n");
            break;
        }
        case AArch64::MLSv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv8i16\n");
            break;
        }
        case AArch64::MLSv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv8i16_indexed\n");
            break;
        }
        case AArch64::MLSv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MLSv8i8\n");
            break;
        }
        case AArch64::MOVID: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVID\n");
            break;
        }
        case AArch64::MOVIv16b_ns: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv16b_ns\n");
            break;
        }
        case AArch64::MOVIv2d_ns: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv2d_ns\n");
            break;
        }
        case AArch64::MOVIv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv2i32\n");
            break;
        }
        case AArch64::MOVIv2s_msl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv2s_msl\n");
            break;
        }
        case AArch64::MOVIv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv4i16\n");
            break;
        }
        case AArch64::MOVIv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv4i32\n");
            break;
        }
        case AArch64::MOVIv4s_msl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv4s_msl\n");
            break;
        }
        case AArch64::MOVIv8b_ns: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv8b_ns\n");
            break;
        }
        case AArch64::MOVIv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVIv8i16\n");
            break;
        }
        case AArch64::MOVKWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVKWi\n");
            break;
        }
        case AArch64::MOVKXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVKXi\n");
            break;
        }
        case AArch64::MOVNWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVNWi\n");
            break;
        }
        case AArch64::MOVNXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVNXi\n");
            break;
        }
        case AArch64::MOVZWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVZWi\n");
            break;
        }
        case AArch64::MOVZXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVZXi\n");
            break;
        }
        case AArch64::MOVaddr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVaddr\n");
            break;
        }
        case AArch64::MOVaddrBA: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVaddrBA\n");
            break;
        }
        case AArch64::MOVaddrCP: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVaddrCP\n");
            break;
        }
        case AArch64::MOVaddrEXT: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVaddrEXT\n");
            break;
        }
        case AArch64::MOVaddrJT: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVaddrJT\n");
            break;
        }
        case AArch64::MOVaddrTLS: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVaddrTLS\n");
            break;
        }
        case AArch64::MOVi32imm: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVi32imm\n");
            break;
        }
        case AArch64::MOVi64imm: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MOVi64imm\n");
            break;
        }
        case AArch64::MRS: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MRS\n");
            break;
        }
        case AArch64::MSR: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MSR\n");
            break;
        }
        case AArch64::MSRpstate: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MSRpstate\n");
            break;
        }
        case AArch64::MSUBWrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MSUBWrrr\n");
            break;
        }
        case AArch64::MSUBXrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MSUBXrrr\n");
            break;
        }
        case AArch64::MULv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv16i8\n");
            break;
        }
        case AArch64::MULv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv2i32\n");
            break;
        }
        case AArch64::MULv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv2i32_indexed\n");
            break;
        }
        case AArch64::MULv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv4i16\n");
            break;
        }
        case AArch64::MULv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv4i16_indexed\n");
            break;
        }
        case AArch64::MULv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv4i32\n");
            break;
        }
        case AArch64::MULv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv4i32_indexed\n");
            break;
        }
        case AArch64::MULv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv8i16\n");
            break;
        }
        case AArch64::MULv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv8i16_indexed\n");
            break;
        }
        case AArch64::MULv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MULv8i8\n");
            break;
        }
        case AArch64::MVNIv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MVNIv2i32\n");
            break;
        }
        case AArch64::MVNIv2s_msl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MVNIv2s_msl\n");
            break;
        }
        case AArch64::MVNIv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MVNIv4i16\n");
            break;
        }
        case AArch64::MVNIv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MVNIv4i32\n");
            break;
        }
        case AArch64::MVNIv4s_msl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MVNIv4s_msl\n");
            break;
        }
        case AArch64::MVNIv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " MVNIv8i16\n");
            break;
        }
        case AArch64::NEGv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv16i8\n");
            break;
        }
        case AArch64::NEGv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv1i64\n");
            break;
        }
        case AArch64::NEGv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv2i32\n");
            break;
        }
        case AArch64::NEGv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv2i64\n");
            break;
        }
        case AArch64::NEGv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv4i16\n");
            break;
        }
        case AArch64::NEGv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv4i32\n");
            break;
        }
        case AArch64::NEGv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv8i16\n");
            break;
        }
        case AArch64::NEGv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NEGv8i8\n");
            break;
        }
        case AArch64::NOTv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NOTv16i8\n");
            break;
        }
        case AArch64::NOTv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " NOTv8i8\n");
            break;
        }
        case AArch64::ORNWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORNWrr\n");
            break;
        }
        case AArch64::ORNWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORNWrs\n");
            break;
        }
        case AArch64::ORNXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORNXrr\n");
            break;
        }
        case AArch64::ORNXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORNXrs\n");
            break;
        }
        case AArch64::ORNv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORNv16i8\n");
            break;
        }
        case AArch64::ORNv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORNv8i8\n");
            break;
        }
        case AArch64::ORRWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRWri\n");
            break;
        }
        case AArch64::ORRWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRWrr\n");
            break;
        }
        case AArch64::ORRWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRWrs\n");
            break;
        }
        case AArch64::ORRXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRXri\n");
            break;
        }
        case AArch64::ORRXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRXrr\n");
            break;
        }
        case AArch64::ORRXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRXrs\n");
            break;
        }
        case AArch64::ORRv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRv16i8\n");
            break;
        }
        case AArch64::ORRv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRv2i32\n");
            break;
        }
        case AArch64::ORRv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRv4i16\n");
            break;
        }
        case AArch64::ORRv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRv4i32\n");
            break;
        }
        case AArch64::ORRv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRv8i16\n");
            break;
        }
        case AArch64::ORRv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ORRv8i8\n");
            break;
        }
        case AArch64::PMULLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PMULLv16i8\n");
            break;
        }
        case AArch64::PMULLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PMULLv1i64\n");
            break;
        }
        case AArch64::PMULLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PMULLv2i64\n");
            break;
        }
        case AArch64::PMULLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PMULLv8i8\n");
            break;
        }
        case AArch64::PMULv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PMULv16i8\n");
            break;
        }
        case AArch64::PMULv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PMULv8i8\n");
            break;
        }
        case AArch64::PRFMl: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PRFMl\n");
            break;
        }
        case AArch64::PRFMroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PRFMroW\n");
            break;
        }
        case AArch64::PRFMroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PRFMroX\n");
            break;
        }
        case AArch64::PRFMui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PRFMui\n");
            break;
        }
        case AArch64::PRFUMi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " PRFUMi\n");
            break;
        }
        case AArch64::RADDHNv2i64_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RADDHNv2i64_v2i32\n");
            break;
        }
        case AArch64::RADDHNv2i64_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RADDHNv2i64_v4i32\n");
            break;
        }
        case AArch64::RADDHNv4i32_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RADDHNv4i32_v4i16\n");
            break;
        }
        case AArch64::RADDHNv4i32_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RADDHNv4i32_v8i16\n");
            break;
        }
        case AArch64::RADDHNv8i16_v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RADDHNv8i16_v16i8\n");
            break;
        }
        case AArch64::RADDHNv8i16_v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RADDHNv8i16_v8i8\n");
            break;
        }
        case AArch64::RBITWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RBITWr\n");
            break;
        }
        case AArch64::RBITXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RBITXr\n");
            break;
        }
        case AArch64::RBITv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RBITv16i8\n");
            break;
        }
        case AArch64::RBITv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RBITv8i8\n");
            break;
        }
        case AArch64::RET: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RET\n");
            break;
        }
        case AArch64::RET_ReallyLR: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RET_ReallyLR\n");
            break;
        }
        case AArch64::REV16Wr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV16Wr\n");
            break;
        }
        case AArch64::REV16Xr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV16Xr\n");
            break;
        }
        case AArch64::REV16v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV16v16i8\n");
            break;
        }
        case AArch64::REV16v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV16v8i8\n");
            break;
        }
        case AArch64::REV32Xr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV32Xr\n");
            break;
        }
        case AArch64::REV32v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV32v16i8\n");
            break;
        }
        case AArch64::REV32v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV32v4i16\n");
            break;
        }
        case AArch64::REV32v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV32v8i16\n");
            break;
        }
        case AArch64::REV32v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV32v8i8\n");
            break;
        }
        case AArch64::REV64v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV64v16i8\n");
            break;
        }
        case AArch64::REV64v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV64v2i32\n");
            break;
        }
        case AArch64::REV64v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV64v4i16\n");
            break;
        }
        case AArch64::REV64v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV64v4i32\n");
            break;
        }
        case AArch64::REV64v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV64v8i16\n");
            break;
        }
        case AArch64::REV64v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REV64v8i8\n");
            break;
        }
        case AArch64::REVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REVWr\n");
            break;
        }
        case AArch64::REVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " REVXr\n");
            break;
        }
        case AArch64::RORVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RORVWr\n");
            break;
        }
        case AArch64::RORVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RORVXr\n");
            break;
        }
        case AArch64::RSHRNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSHRNv16i8_shift\n");
            break;
        }
        case AArch64::RSHRNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSHRNv2i32_shift\n");
            break;
        }
        case AArch64::RSHRNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSHRNv4i16_shift\n");
            break;
        }
        case AArch64::RSHRNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSHRNv4i32_shift\n");
            break;
        }
        case AArch64::RSHRNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSHRNv8i16_shift\n");
            break;
        }
        case AArch64::RSHRNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSHRNv8i8_shift\n");
            break;
        }
        case AArch64::RSUBHNv2i64_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSUBHNv2i64_v2i32\n");
            break;
        }
        case AArch64::RSUBHNv2i64_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSUBHNv2i64_v4i32\n");
            break;
        }
        case AArch64::RSUBHNv4i32_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSUBHNv4i32_v4i16\n");
            break;
        }
        case AArch64::RSUBHNv4i32_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSUBHNv4i32_v8i16\n");
            break;
        }
        case AArch64::RSUBHNv8i16_v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSUBHNv8i16_v16i8\n");
            break;
        }
        case AArch64::RSUBHNv8i16_v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " RSUBHNv8i16_v8i8\n");
            break;
        }
        case AArch64::SABALv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABALv16i8_v8i16\n");
            break;
        }
        case AArch64::SABALv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABALv2i32_v2i64\n");
            break;
        }
        case AArch64::SABALv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABALv4i16_v4i32\n");
            break;
        }
        case AArch64::SABALv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABALv4i32_v2i64\n");
            break;
        }
        case AArch64::SABALv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABALv8i16_v4i32\n");
            break;
        }
        case AArch64::SABALv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABALv8i8_v8i16\n");
            break;
        }
        case AArch64::SABAv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABAv16i8\n");
            break;
        }
        case AArch64::SABAv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABAv2i32\n");
            break;
        }
        case AArch64::SABAv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABAv4i16\n");
            break;
        }
        case AArch64::SABAv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABAv4i32\n");
            break;
        }
        case AArch64::SABAv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABAv8i16\n");
            break;
        }
        case AArch64::SABAv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABAv8i8\n");
            break;
        }
        case AArch64::SABDLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDLv16i8_v8i16\n");
            break;
        }
        case AArch64::SABDLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDLv2i32_v2i64\n");
            break;
        }
        case AArch64::SABDLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDLv4i16_v4i32\n");
            break;
        }
        case AArch64::SABDLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDLv4i32_v2i64\n");
            break;
        }
        case AArch64::SABDLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDLv8i16_v4i32\n");
            break;
        }
        case AArch64::SABDLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDLv8i8_v8i16\n");
            break;
        }
        case AArch64::SABDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDv16i8\n");
            break;
        }
        case AArch64::SABDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDv2i32\n");
            break;
        }
        case AArch64::SABDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDv4i16\n");
            break;
        }
        case AArch64::SABDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDv4i32\n");
            break;
        }
        case AArch64::SABDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDv8i16\n");
            break;
        }
        case AArch64::SABDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SABDv8i8\n");
            break;
        }
        case AArch64::SADALPv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADALPv16i8_v8i16\n");
            break;
        }
        case AArch64::SADALPv2i32_v1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADALPv2i32_v1i64\n");
            break;
        }
        case AArch64::SADALPv4i16_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADALPv4i16_v2i32\n");
            break;
        }
        case AArch64::SADALPv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADALPv4i32_v2i64\n");
            break;
        }
        case AArch64::SADALPv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADALPv8i16_v4i32\n");
            break;
        }
        case AArch64::SADALPv8i8_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADALPv8i8_v4i16\n");
            break;
        }
        case AArch64::SADDLPv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLPv16i8_v8i16\n");
            break;
        }
        case AArch64::SADDLPv2i32_v1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLPv2i32_v1i64\n");
            break;
        }
        case AArch64::SADDLPv4i16_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLPv4i16_v2i32\n");
            break;
        }
        case AArch64::SADDLPv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLPv4i32_v2i64\n");
            break;
        }
        case AArch64::SADDLPv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLPv8i16_v4i32\n");
            break;
        }
        case AArch64::SADDLPv8i8_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLPv8i8_v4i16\n");
            break;
        }
        case AArch64::SADDLVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLVv16i8v\n");
            break;
        }
        case AArch64::SADDLVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLVv4i16v\n");
            break;
        }
        case AArch64::SADDLVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLVv4i32v\n");
            break;
        }
        case AArch64::SADDLVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLVv8i16v\n");
            break;
        }
        case AArch64::SADDLVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLVv8i8v\n");
            break;
        }
        case AArch64::SADDLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLv16i8_v8i16\n");
            break;
        }
        case AArch64::SADDLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLv2i32_v2i64\n");
            break;
        }
        case AArch64::SADDLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLv4i16_v4i32\n");
            break;
        }
        case AArch64::SADDLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLv4i32_v2i64\n");
            break;
        }
        case AArch64::SADDLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLv8i16_v4i32\n");
            break;
        }
        case AArch64::SADDLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDLv8i8_v8i16\n");
            break;
        }
        case AArch64::SADDWv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDWv16i8_v8i16\n");
            break;
        }
        case AArch64::SADDWv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDWv2i32_v2i64\n");
            break;
        }
        case AArch64::SADDWv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDWv4i16_v4i32\n");
            break;
        }
        case AArch64::SADDWv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDWv4i32_v2i64\n");
            break;
        }
        case AArch64::SADDWv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDWv8i16_v4i32\n");
            break;
        }
        case AArch64::SADDWv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SADDWv8i8_v8i16\n");
            break;
        }
        case AArch64::SBCSWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SBCSWr\n");
            break;
        }
        case AArch64::SBCSXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SBCSXr\n");
            break;
        }
        case AArch64::SBCWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SBCWr\n");
            break;
        }
        case AArch64::SBCXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SBCXr\n");
            break;
        }
        case AArch64::SBFMWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SBFMWri\n");
            break;
        }
        case AArch64::SBFMXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SBFMXri\n");
            break;
        }
        case AArch64::SCVTFSWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFSWDri\n");
            break;
        }
        case AArch64::SCVTFSWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFSWSri\n");
            break;
        }
        case AArch64::SCVTFSXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFSXDri\n");
            break;
        }
        case AArch64::SCVTFSXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFSXSri\n");
            break;
        }
        case AArch64::SCVTFUWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFUWDri\n");
            break;
        }
        case AArch64::SCVTFUWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFUWSri\n");
            break;
        }
        case AArch64::SCVTFUXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFUXDri\n");
            break;
        }
        case AArch64::SCVTFUXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFUXSri\n");
            break;
        }
        case AArch64::SCVTFd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFd\n");
            break;
        }
        case AArch64::SCVTFs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFs\n");
            break;
        }
        case AArch64::SCVTFv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv1i32\n");
            break;
        }
        case AArch64::SCVTFv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv1i64\n");
            break;
        }
        case AArch64::SCVTFv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv2f32\n");
            break;
        }
        case AArch64::SCVTFv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv2f64\n");
            break;
        }
        case AArch64::SCVTFv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv2i32_shift\n");
            break;
        }
        case AArch64::SCVTFv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv2i64_shift\n");
            break;
        }
        case AArch64::SCVTFv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv4f32\n");
            break;
        }
        case AArch64::SCVTFv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SCVTFv4i32_shift\n");
            break;
        }
        case AArch64::SDIVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SDIVWr\n");
            break;
        }
        case AArch64::SDIVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SDIVXr\n");
            break;
        }
        case AArch64::SDIV_IntWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SDIV_IntWr\n");
            break;
        }
        case AArch64::SDIV_IntXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SDIV_IntXr\n");
            break;
        }
        case AArch64::SHA1Crrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA1Crrr\n");
            break;
        }
        case AArch64::SHA1Hrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA1Hrr\n");
            break;
        }
        case AArch64::SHA1Mrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA1Mrrr\n");
            break;
        }
        case AArch64::SHA1Prrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA1Prrr\n");
            break;
        }
        case AArch64::SHA1SU0rrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA1SU0rrr\n");
            break;
        }
        case AArch64::SHA1SU1rr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA1SU1rr\n");
            break;
        }
        case AArch64::SHA256H2rrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA256H2rrr\n");
            break;
        }
        case AArch64::SHA256Hrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA256Hrrr\n");
            break;
        }
        case AArch64::SHA256SU0rr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA256SU0rr\n");
            break;
        }
        case AArch64::SHA256SU1rrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHA256SU1rrr\n");
            break;
        }
        case AArch64::SHADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHADDv16i8\n");
            break;
        }
        case AArch64::SHADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHADDv2i32\n");
            break;
        }
        case AArch64::SHADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHADDv4i16\n");
            break;
        }
        case AArch64::SHADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHADDv4i32\n");
            break;
        }
        case AArch64::SHADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHADDv8i16\n");
            break;
        }
        case AArch64::SHADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHADDv8i8\n");
            break;
        }
        case AArch64::SHLLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLLv16i8\n");
            break;
        }
        case AArch64::SHLLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLLv2i32\n");
            break;
        }
        case AArch64::SHLLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLLv4i16\n");
            break;
        }
        case AArch64::SHLLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLLv4i32\n");
            break;
        }
        case AArch64::SHLLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLLv8i16\n");
            break;
        }
        case AArch64::SHLLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLLv8i8\n");
            break;
        }
        case AArch64::SHLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLd\n");
            break;
        }
        case AArch64::SHLv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv16i8_shift\n");
            break;
        }
        case AArch64::SHLv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv2i32_shift\n");
            break;
        }
        case AArch64::SHLv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv2i64_shift\n");
            break;
        }
        case AArch64::SHLv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv4i16_shift\n");
            break;
        }
        case AArch64::SHLv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv4i32_shift\n");
            break;
        }
        case AArch64::SHLv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv8i16_shift\n");
            break;
        }
        case AArch64::SHLv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHLv8i8_shift\n");
            break;
        }
        case AArch64::SHRNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHRNv16i8_shift\n");
            break;
        }
        case AArch64::SHRNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHRNv2i32_shift\n");
            break;
        }
        case AArch64::SHRNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHRNv4i16_shift\n");
            break;
        }
        case AArch64::SHRNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHRNv4i32_shift\n");
            break;
        }
        case AArch64::SHRNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHRNv8i16_shift\n");
            break;
        }
        case AArch64::SHRNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHRNv8i8_shift\n");
            break;
        }
        case AArch64::SHSUBv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHSUBv16i8\n");
            break;
        }
        case AArch64::SHSUBv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHSUBv2i32\n");
            break;
        }
        case AArch64::SHSUBv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHSUBv4i16\n");
            break;
        }
        case AArch64::SHSUBv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHSUBv4i32\n");
            break;
        }
        case AArch64::SHSUBv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHSUBv8i16\n");
            break;
        }
        case AArch64::SHSUBv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SHSUBv8i8\n");
            break;
        }
        case AArch64::SLId: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLId\n");
            break;
        }
        case AArch64::SLIv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv16i8_shift\n");
            break;
        }
        case AArch64::SLIv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv2i32_shift\n");
            break;
        }
        case AArch64::SLIv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv2i64_shift\n");
            break;
        }
        case AArch64::SLIv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv4i16_shift\n");
            break;
        }
        case AArch64::SLIv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv4i32_shift\n");
            break;
        }
        case AArch64::SLIv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv8i16_shift\n");
            break;
        }
        case AArch64::SLIv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SLIv8i8_shift\n");
            break;
        }
        case AArch64::SMADDLrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMADDLrrr\n");
            break;
        }
        case AArch64::SMAXPv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXPv16i8\n");
            break;
        }
        case AArch64::SMAXPv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXPv2i32\n");
            break;
        }
        case AArch64::SMAXPv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXPv4i16\n");
            break;
        }
        case AArch64::SMAXPv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXPv4i32\n");
            break;
        }
        case AArch64::SMAXPv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXPv8i16\n");
            break;
        }
        case AArch64::SMAXPv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXPv8i8\n");
            break;
        }
        case AArch64::SMAXVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXVv16i8v\n");
            break;
        }
        case AArch64::SMAXVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXVv4i16v\n");
            break;
        }
        case AArch64::SMAXVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXVv4i32v\n");
            break;
        }
        case AArch64::SMAXVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXVv8i16v\n");
            break;
        }
        case AArch64::SMAXVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXVv8i8v\n");
            break;
        }
        case AArch64::SMAXv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXv16i8\n");
            break;
        }
        case AArch64::SMAXv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXv2i32\n");
            break;
        }
        case AArch64::SMAXv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXv4i16\n");
            break;
        }
        case AArch64::SMAXv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXv4i32\n");
            break;
        }
        case AArch64::SMAXv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXv8i16\n");
            break;
        }
        case AArch64::SMAXv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMAXv8i8\n");
            break;
        }
        case AArch64::SMC: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMC\n");
            break;
        }
        case AArch64::SMINPv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINPv16i8\n");
            break;
        }
        case AArch64::SMINPv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINPv2i32\n");
            break;
        }
        case AArch64::SMINPv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINPv4i16\n");
            break;
        }
        case AArch64::SMINPv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINPv4i32\n");
            break;
        }
        case AArch64::SMINPv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINPv8i16\n");
            break;
        }
        case AArch64::SMINPv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINPv8i8\n");
            break;
        }
        case AArch64::SMINVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINVv16i8v\n");
            break;
        }
        case AArch64::SMINVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINVv4i16v\n");
            break;
        }
        case AArch64::SMINVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINVv4i32v\n");
            break;
        }
        case AArch64::SMINVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINVv8i16v\n");
            break;
        }
        case AArch64::SMINVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINVv8i8v\n");
            break;
        }
        case AArch64::SMINv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINv16i8\n");
            break;
        }
        case AArch64::SMINv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINv2i32\n");
            break;
        }
        case AArch64::SMINv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINv4i16\n");
            break;
        }
        case AArch64::SMINv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINv4i32\n");
            break;
        }
        case AArch64::SMINv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINv8i16\n");
            break;
        }
        case AArch64::SMINv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMINv8i8\n");
            break;
        }
        case AArch64::SMLALv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv16i8_v8i16\n");
            break;
        }
        case AArch64::SMLALv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv2i32_indexed\n");
            break;
        }
        case AArch64::SMLALv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv2i32_v2i64\n");
            break;
        }
        case AArch64::SMLALv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv4i16_indexed\n");
            break;
        }
        case AArch64::SMLALv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv4i16_v4i32\n");
            break;
        }
        case AArch64::SMLALv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv4i32_indexed\n");
            break;
        }
        case AArch64::SMLALv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv4i32_v2i64\n");
            break;
        }
        case AArch64::SMLALv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv8i16_indexed\n");
            break;
        }
        case AArch64::SMLALv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv8i16_v4i32\n");
            break;
        }
        case AArch64::SMLALv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLALv8i8_v8i16\n");
            break;
        }
        case AArch64::SMLSLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv16i8_v8i16\n");
            break;
        }
        case AArch64::SMLSLv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv2i32_indexed\n");
            break;
        }
        case AArch64::SMLSLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv2i32_v2i64\n");
            break;
        }
        case AArch64::SMLSLv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv4i16_indexed\n");
            break;
        }
        case AArch64::SMLSLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv4i16_v4i32\n");
            break;
        }
        case AArch64::SMLSLv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv4i32_indexed\n");
            break;
        }
        case AArch64::SMLSLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv4i32_v2i64\n");
            break;
        }
        case AArch64::SMLSLv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv8i16_indexed\n");
            break;
        }
        case AArch64::SMLSLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv8i16_v4i32\n");
            break;
        }
        case AArch64::SMLSLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMLSLv8i8_v8i16\n");
            break;
        }
        case AArch64::SMOVvi16to32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMOVvi16to32\n");
            break;
        }
        case AArch64::SMOVvi16to64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMOVvi16to64\n");
            break;
        }
        case AArch64::SMOVvi32to64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMOVvi32to64\n");
            break;
        }
        case AArch64::SMOVvi8to32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMOVvi8to32\n");
            break;
        }
        case AArch64::SMOVvi8to64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMOVvi8to64\n");
            break;
        }
        case AArch64::SMSUBLrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMSUBLrrr\n");
            break;
        }
        case AArch64::SMULHrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULHrr\n");
            break;
        }
        case AArch64::SMULLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv16i8_v8i16\n");
            break;
        }
        case AArch64::SMULLv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv2i32_indexed\n");
            break;
        }
        case AArch64::SMULLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv2i32_v2i64\n");
            break;
        }
        case AArch64::SMULLv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv4i16_indexed\n");
            break;
        }
        case AArch64::SMULLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv4i16_v4i32\n");
            break;
        }
        case AArch64::SMULLv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv4i32_indexed\n");
            break;
        }
        case AArch64::SMULLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv4i32_v2i64\n");
            break;
        }
        case AArch64::SMULLv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv8i16_indexed\n");
            break;
        }
        case AArch64::SMULLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv8i16_v4i32\n");
            break;
        }
        case AArch64::SMULLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SMULLv8i8_v8i16\n");
            break;
        }
        case AArch64::SQABSv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv16i8\n");
            break;
        }
        case AArch64::SQABSv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv1i16\n");
            break;
        }
        case AArch64::SQABSv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv1i32\n");
            break;
        }
        case AArch64::SQABSv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv1i64\n");
            break;
        }
        case AArch64::SQABSv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv1i8\n");
            break;
        }
        case AArch64::SQABSv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv2i32\n");
            break;
        }
        case AArch64::SQABSv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv2i64\n");
            break;
        }
        case AArch64::SQABSv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv4i16\n");
            break;
        }
        case AArch64::SQABSv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv4i32\n");
            break;
        }
        case AArch64::SQABSv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv8i16\n");
            break;
        }
        case AArch64::SQABSv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQABSv8i8\n");
            break;
        }
        case AArch64::SQADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv16i8\n");
            break;
        }
        case AArch64::SQADDv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv1i16\n");
            break;
        }
        case AArch64::SQADDv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv1i32\n");
            break;
        }
        case AArch64::SQADDv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv1i64\n");
            break;
        }
        case AArch64::SQADDv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv1i8\n");
            break;
        }
        case AArch64::SQADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv2i32\n");
            break;
        }
        case AArch64::SQADDv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv2i64\n");
            break;
        }
        case AArch64::SQADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv4i16\n");
            break;
        }
        case AArch64::SQADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv4i32\n");
            break;
        }
        case AArch64::SQADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv8i16\n");
            break;
        }
        case AArch64::SQADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQADDv8i8\n");
            break;
        }
        case AArch64::SQDMLALi16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALi16\n");
            break;
        }
        case AArch64::SQDMLALi32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALi32\n");
            break;
        }
        case AArch64::SQDMLALv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv1i32_indexed\n");
            break;
        }
        case AArch64::SQDMLALv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv1i64_indexed\n");
            break;
        }
        case AArch64::SQDMLALv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv2i32_indexed\n");
            break;
        }
        case AArch64::SQDMLALv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv2i32_v2i64\n");
            break;
        }
        case AArch64::SQDMLALv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv4i16_indexed\n");
            break;
        }
        case AArch64::SQDMLALv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv4i16_v4i32\n");
            break;
        }
        case AArch64::SQDMLALv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv4i32_indexed\n");
            break;
        }
        case AArch64::SQDMLALv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv4i32_v2i64\n");
            break;
        }
        case AArch64::SQDMLALv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv8i16_indexed\n");
            break;
        }
        case AArch64::SQDMLALv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLALv8i16_v4i32\n");
            break;
        }
        case AArch64::SQDMLSLi16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLi16\n");
            break;
        }
        case AArch64::SQDMLSLi32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLi32\n");
            break;
        }
        case AArch64::SQDMLSLv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv1i32_indexed\n");
            break;
        }
        case AArch64::SQDMLSLv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv1i64_indexed\n");
            break;
        }
        case AArch64::SQDMLSLv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv2i32_indexed\n");
            break;
        }
        case AArch64::SQDMLSLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv2i32_v2i64\n");
            break;
        }
        case AArch64::SQDMLSLv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv4i16_indexed\n");
            break;
        }
        case AArch64::SQDMLSLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv4i16_v4i32\n");
            break;
        }
        case AArch64::SQDMLSLv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv4i32_indexed\n");
            break;
        }
        case AArch64::SQDMLSLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv4i32_v2i64\n");
            break;
        }
        case AArch64::SQDMLSLv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv8i16_indexed\n");
            break;
        }
        case AArch64::SQDMLSLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMLSLv8i16_v4i32\n");
            break;
        }
        case AArch64::SQDMULHv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv1i16\n");
            break;
        }
        case AArch64::SQDMULHv1i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv1i16_indexed\n");
            break;
        }
        case AArch64::SQDMULHv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv1i32\n");
            break;
        }
        case AArch64::SQDMULHv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv1i32_indexed\n");
            break;
        }
        case AArch64::SQDMULHv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv2i32\n");
            break;
        }
        case AArch64::SQDMULHv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv2i32_indexed\n");
            break;
        }
        case AArch64::SQDMULHv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv4i16\n");
            break;
        }
        case AArch64::SQDMULHv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv4i16_indexed\n");
            break;
        }
        case AArch64::SQDMULHv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv4i32\n");
            break;
        }
        case AArch64::SQDMULHv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv4i32_indexed\n");
            break;
        }
        case AArch64::SQDMULHv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv8i16\n");
            break;
        }
        case AArch64::SQDMULHv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULHv8i16_indexed\n");
            break;
        }
        case AArch64::SQDMULLi16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLi16\n");
            break;
        }
        case AArch64::SQDMULLi32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLi32\n");
            break;
        }
        case AArch64::SQDMULLv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv1i32_indexed\n");
            break;
        }
        case AArch64::SQDMULLv1i64_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv1i64_indexed\n");
            break;
        }
        case AArch64::SQDMULLv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv2i32_indexed\n");
            break;
        }
        case AArch64::SQDMULLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv2i32_v2i64\n");
            break;
        }
        case AArch64::SQDMULLv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv4i16_indexed\n");
            break;
        }
        case AArch64::SQDMULLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv4i16_v4i32\n");
            break;
        }
        case AArch64::SQDMULLv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv4i32_indexed\n");
            break;
        }
        case AArch64::SQDMULLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv4i32_v2i64\n");
            break;
        }
        case AArch64::SQDMULLv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv8i16_indexed\n");
            break;
        }
        case AArch64::SQDMULLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQDMULLv8i16_v4i32\n");
            break;
        }
        case AArch64::SQNEGv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv16i8\n");
            break;
        }
        case AArch64::SQNEGv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv1i16\n");
            break;
        }
        case AArch64::SQNEGv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv1i32\n");
            break;
        }
        case AArch64::SQNEGv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv1i64\n");
            break;
        }
        case AArch64::SQNEGv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv1i8\n");
            break;
        }
        case AArch64::SQNEGv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv2i32\n");
            break;
        }
        case AArch64::SQNEGv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv2i64\n");
            break;
        }
        case AArch64::SQNEGv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv4i16\n");
            break;
        }
        case AArch64::SQNEGv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv4i32\n");
            break;
        }
        case AArch64::SQNEGv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv8i16\n");
            break;
        }
        case AArch64::SQNEGv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQNEGv8i8\n");
            break;
        }
        case AArch64::SQRDMLAHi16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHi16_indexed\n");
            break;
        }
        case AArch64::SQRDMLAHi32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHi32_indexed\n");
            break;
        }
        case AArch64::SQRDMLAHv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv1i16\n");
            break;
        }
        case AArch64::SQRDMLAHv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv1i32\n");
            break;
        }
        case AArch64::SQRDMLAHv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv2i32\n");
            break;
        }
        case AArch64::SQRDMLAHv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv2i32_indexed\n");
            break;
        }
        case AArch64::SQRDMLAHv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv4i16\n");
            break;
        }
        case AArch64::SQRDMLAHv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv4i16_indexed\n");
            break;
        }
        case AArch64::SQRDMLAHv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv4i32\n");
            break;
        }
        case AArch64::SQRDMLAHv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv4i32_indexed\n");
            break;
        }
        case AArch64::SQRDMLAHv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv8i16\n");
            break;
        }
        case AArch64::SQRDMLAHv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLAHv8i16_indexed\n");
            break;
        }
        case AArch64::SQRDMLSHi16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHi16_indexed\n");
            break;
        }
        case AArch64::SQRDMLSHi32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHi32_indexed\n");
            break;
        }
        case AArch64::SQRDMLSHv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv1i16\n");
            break;
        }
        case AArch64::SQRDMLSHv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv1i32\n");
            break;
        }
        case AArch64::SQRDMLSHv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv2i32\n");
            break;
        }
        case AArch64::SQRDMLSHv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv2i32_indexed\n");
            break;
        }
        case AArch64::SQRDMLSHv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv4i16\n");
            break;
        }
        case AArch64::SQRDMLSHv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv4i16_indexed\n");
            break;
        }
        case AArch64::SQRDMLSHv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv4i32\n");
            break;
        }
        case AArch64::SQRDMLSHv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv4i32_indexed\n");
            break;
        }
        case AArch64::SQRDMLSHv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv8i16\n");
            break;
        }
        case AArch64::SQRDMLSHv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMLSHv8i16_indexed\n");
            break;
        }
        case AArch64::SQRDMULHv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv1i16\n");
            break;
        }
        case AArch64::SQRDMULHv1i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv1i16_indexed\n");
            break;
        }
        case AArch64::SQRDMULHv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv1i32\n");
            break;
        }
        case AArch64::SQRDMULHv1i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv1i32_indexed\n");
            break;
        }
        case AArch64::SQRDMULHv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv2i32\n");
            break;
        }
        case AArch64::SQRDMULHv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv2i32_indexed\n");
            break;
        }
        case AArch64::SQRDMULHv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv4i16\n");
            break;
        }
        case AArch64::SQRDMULHv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv4i16_indexed\n");
            break;
        }
        case AArch64::SQRDMULHv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv4i32\n");
            break;
        }
        case AArch64::SQRDMULHv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv4i32_indexed\n");
            break;
        }
        case AArch64::SQRDMULHv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv8i16\n");
            break;
        }
        case AArch64::SQRDMULHv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRDMULHv8i16_indexed\n");
            break;
        }
        case AArch64::SQRSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv16i8\n");
            break;
        }
        case AArch64::SQRSHLv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv1i16\n");
            break;
        }
        case AArch64::SQRSHLv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv1i32\n");
            break;
        }
        case AArch64::SQRSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv1i64\n");
            break;
        }
        case AArch64::SQRSHLv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv1i8\n");
            break;
        }
        case AArch64::SQRSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv2i32\n");
            break;
        }
        case AArch64::SQRSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv2i64\n");
            break;
        }
        case AArch64::SQRSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv4i16\n");
            break;
        }
        case AArch64::SQRSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv4i32\n");
            break;
        }
        case AArch64::SQRSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv8i16\n");
            break;
        }
        case AArch64::SQRSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHLv8i8\n");
            break;
        }
        case AArch64::SQRSHRNb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNb\n");
            break;
        }
        case AArch64::SQRSHRNh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNh\n");
            break;
        }
        case AArch64::SQRSHRNs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNs\n");
            break;
        }
        case AArch64::SQRSHRNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNv16i8_shift\n");
            break;
        }
        case AArch64::SQRSHRNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNv2i32_shift\n");
            break;
        }
        case AArch64::SQRSHRNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNv4i16_shift\n");
            break;
        }
        case AArch64::SQRSHRNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNv4i32_shift\n");
            break;
        }
        case AArch64::SQRSHRNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNv8i16_shift\n");
            break;
        }
        case AArch64::SQRSHRNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRNv8i8_shift\n");
            break;
        }
        case AArch64::SQRSHRUNb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNb\n");
            break;
        }
        case AArch64::SQRSHRUNh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNh\n");
            break;
        }
        case AArch64::SQRSHRUNs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNs\n");
            break;
        }
        case AArch64::SQRSHRUNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNv16i8_shift\n");
            break;
        }
        case AArch64::SQRSHRUNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNv2i32_shift\n");
            break;
        }
        case AArch64::SQRSHRUNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNv4i16_shift\n");
            break;
        }
        case AArch64::SQRSHRUNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNv4i32_shift\n");
            break;
        }
        case AArch64::SQRSHRUNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNv8i16_shift\n");
            break;
        }
        case AArch64::SQRSHRUNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQRSHRUNv8i8_shift\n");
            break;
        }
        case AArch64::SQSHLUb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUb\n");
            break;
        }
        case AArch64::SQSHLUd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUd\n");
            break;
        }
        case AArch64::SQSHLUh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUh\n");
            break;
        }
        case AArch64::SQSHLUs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUs\n");
            break;
        }
        case AArch64::SQSHLUv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv16i8_shift\n");
            break;
        }
        case AArch64::SQSHLUv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv2i32_shift\n");
            break;
        }
        case AArch64::SQSHLUv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv2i64_shift\n");
            break;
        }
        case AArch64::SQSHLUv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv4i16_shift\n");
            break;
        }
        case AArch64::SQSHLUv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv4i32_shift\n");
            break;
        }
        case AArch64::SQSHLUv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv8i16_shift\n");
            break;
        }
        case AArch64::SQSHLUv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLUv8i8_shift\n");
            break;
        }
        case AArch64::SQSHLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLb\n");
            break;
        }
        case AArch64::SQSHLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLd\n");
            break;
        }
        case AArch64::SQSHLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLh\n");
            break;
        }
        case AArch64::SQSHLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLs\n");
            break;
        }
        case AArch64::SQSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv16i8\n");
            break;
        }
        case AArch64::SQSHLv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv16i8_shift\n");
            break;
        }
        case AArch64::SQSHLv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv1i16\n");
            break;
        }
        case AArch64::SQSHLv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv1i32\n");
            break;
        }
        case AArch64::SQSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv1i64\n");
            break;
        }
        case AArch64::SQSHLv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv1i8\n");
            break;
        }
        case AArch64::SQSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv2i32\n");
            break;
        }
        case AArch64::SQSHLv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv2i32_shift\n");
            break;
        }
        case AArch64::SQSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv2i64\n");
            break;
        }
        case AArch64::SQSHLv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv2i64_shift\n");
            break;
        }
        case AArch64::SQSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv4i16\n");
            break;
        }
        case AArch64::SQSHLv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv4i16_shift\n");
            break;
        }
        case AArch64::SQSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv4i32\n");
            break;
        }
        case AArch64::SQSHLv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv4i32_shift\n");
            break;
        }
        case AArch64::SQSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv8i16\n");
            break;
        }
        case AArch64::SQSHLv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv8i16_shift\n");
            break;
        }
        case AArch64::SQSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv8i8\n");
            break;
        }
        case AArch64::SQSHLv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHLv8i8_shift\n");
            break;
        }
        case AArch64::SQSHRNb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNb\n");
            break;
        }
        case AArch64::SQSHRNh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNh\n");
            break;
        }
        case AArch64::SQSHRNs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNs\n");
            break;
        }
        case AArch64::SQSHRNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNv16i8_shift\n");
            break;
        }
        case AArch64::SQSHRNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNv2i32_shift\n");
            break;
        }
        case AArch64::SQSHRNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNv4i16_shift\n");
            break;
        }
        case AArch64::SQSHRNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNv4i32_shift\n");
            break;
        }
        case AArch64::SQSHRNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNv8i16_shift\n");
            break;
        }
        case AArch64::SQSHRNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRNv8i8_shift\n");
            break;
        }
        case AArch64::SQSHRUNb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNb\n");
            break;
        }
        case AArch64::SQSHRUNh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNh\n");
            break;
        }
        case AArch64::SQSHRUNs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNs\n");
            break;
        }
        case AArch64::SQSHRUNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNv16i8_shift\n");
            break;
        }
        case AArch64::SQSHRUNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNv2i32_shift\n");
            break;
        }
        case AArch64::SQSHRUNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNv4i16_shift\n");
            break;
        }
        case AArch64::SQSHRUNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNv4i32_shift\n");
            break;
        }
        case AArch64::SQSHRUNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNv8i16_shift\n");
            break;
        }
        case AArch64::SQSHRUNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSHRUNv8i8_shift\n");
            break;
        }
        case AArch64::SQSUBv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv16i8\n");
            break;
        }
        case AArch64::SQSUBv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv1i16\n");
            break;
        }
        case AArch64::SQSUBv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv1i32\n");
            break;
        }
        case AArch64::SQSUBv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv1i64\n");
            break;
        }
        case AArch64::SQSUBv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv1i8\n");
            break;
        }
        case AArch64::SQSUBv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv2i32\n");
            break;
        }
        case AArch64::SQSUBv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv2i64\n");
            break;
        }
        case AArch64::SQSUBv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv4i16\n");
            break;
        }
        case AArch64::SQSUBv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv4i32\n");
            break;
        }
        case AArch64::SQSUBv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv8i16\n");
            break;
        }
        case AArch64::SQSUBv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQSUBv8i8\n");
            break;
        }
        case AArch64::SQXTNv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv16i8\n");
            break;
        }
        case AArch64::SQXTNv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv1i16\n");
            break;
        }
        case AArch64::SQXTNv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv1i32\n");
            break;
        }
        case AArch64::SQXTNv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv1i8\n");
            break;
        }
        case AArch64::SQXTNv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv2i32\n");
            break;
        }
        case AArch64::SQXTNv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv4i16\n");
            break;
        }
        case AArch64::SQXTNv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv4i32\n");
            break;
        }
        case AArch64::SQXTNv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv8i16\n");
            break;
        }
        case AArch64::SQXTNv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTNv8i8\n");
            break;
        }
        case AArch64::SQXTUNv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv16i8\n");
            break;
        }
        case AArch64::SQXTUNv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv1i16\n");
            break;
        }
        case AArch64::SQXTUNv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv1i32\n");
            break;
        }
        case AArch64::SQXTUNv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv1i8\n");
            break;
        }
        case AArch64::SQXTUNv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv2i32\n");
            break;
        }
        case AArch64::SQXTUNv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv4i16\n");
            break;
        }
        case AArch64::SQXTUNv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv4i32\n");
            break;
        }
        case AArch64::SQXTUNv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv8i16\n");
            break;
        }
        case AArch64::SQXTUNv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SQXTUNv8i8\n");
            break;
        }
        case AArch64::SRHADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRHADDv16i8\n");
            break;
        }
        case AArch64::SRHADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRHADDv2i32\n");
            break;
        }
        case AArch64::SRHADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRHADDv4i16\n");
            break;
        }
        case AArch64::SRHADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRHADDv4i32\n");
            break;
        }
        case AArch64::SRHADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRHADDv8i16\n");
            break;
        }
        case AArch64::SRHADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRHADDv8i8\n");
            break;
        }
        case AArch64::SRId: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRId\n");
            break;
        }
        case AArch64::SRIv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv16i8_shift\n");
            break;
        }
        case AArch64::SRIv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv2i32_shift\n");
            break;
        }
        case AArch64::SRIv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv2i64_shift\n");
            break;
        }
        case AArch64::SRIv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv4i16_shift\n");
            break;
        }
        case AArch64::SRIv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv4i32_shift\n");
            break;
        }
        case AArch64::SRIv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv8i16_shift\n");
            break;
        }
        case AArch64::SRIv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRIv8i8_shift\n");
            break;
        }
        case AArch64::SRSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv16i8\n");
            break;
        }
        case AArch64::SRSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv1i64\n");
            break;
        }
        case AArch64::SRSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv2i32\n");
            break;
        }
        case AArch64::SRSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv2i64\n");
            break;
        }
        case AArch64::SRSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv4i16\n");
            break;
        }
        case AArch64::SRSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv4i32\n");
            break;
        }
        case AArch64::SRSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv8i16\n");
            break;
        }
        case AArch64::SRSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHLv8i8\n");
            break;
        }
        case AArch64::SRSHRd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRd\n");
            break;
        }
        case AArch64::SRSHRv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv16i8_shift\n");
            break;
        }
        case AArch64::SRSHRv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv2i32_shift\n");
            break;
        }
        case AArch64::SRSHRv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv2i64_shift\n");
            break;
        }
        case AArch64::SRSHRv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv4i16_shift\n");
            break;
        }
        case AArch64::SRSHRv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv4i32_shift\n");
            break;
        }
        case AArch64::SRSHRv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv8i16_shift\n");
            break;
        }
        case AArch64::SRSHRv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSHRv8i8_shift\n");
            break;
        }
        case AArch64::SRSRAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAd\n");
            break;
        }
        case AArch64::SRSRAv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv16i8_shift\n");
            break;
        }
        case AArch64::SRSRAv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv2i32_shift\n");
            break;
        }
        case AArch64::SRSRAv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv2i64_shift\n");
            break;
        }
        case AArch64::SRSRAv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv4i16_shift\n");
            break;
        }
        case AArch64::SRSRAv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv4i32_shift\n");
            break;
        }
        case AArch64::SRSRAv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv8i16_shift\n");
            break;
        }
        case AArch64::SRSRAv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SRSRAv8i8_shift\n");
            break;
        }
        case AArch64::SSHLLv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLLv16i8_shift\n");
            break;
        }
        case AArch64::SSHLLv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLLv2i32_shift\n");
            break;
        }
        case AArch64::SSHLLv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLLv4i16_shift\n");
            break;
        }
        case AArch64::SSHLLv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLLv4i32_shift\n");
            break;
        }
        case AArch64::SSHLLv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLLv8i16_shift\n");
            break;
        }
        case AArch64::SSHLLv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLLv8i8_shift\n");
            break;
        }
        case AArch64::SSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv16i8\n");
            break;
        }
        case AArch64::SSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv1i64\n");
            break;
        }
        case AArch64::SSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv2i32\n");
            break;
        }
        case AArch64::SSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv2i64\n");
            break;
        }
        case AArch64::SSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv4i16\n");
            break;
        }
        case AArch64::SSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv4i32\n");
            break;
        }
        case AArch64::SSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv8i16\n");
            break;
        }
        case AArch64::SSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHLv8i8\n");
            break;
        }
        case AArch64::SSHRd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRd\n");
            break;
        }
        case AArch64::SSHRv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv16i8_shift\n");
            break;
        }
        case AArch64::SSHRv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv2i32_shift\n");
            break;
        }
        case AArch64::SSHRv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv2i64_shift\n");
            break;
        }
        case AArch64::SSHRv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv4i16_shift\n");
            break;
        }
        case AArch64::SSHRv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv4i32_shift\n");
            break;
        }
        case AArch64::SSHRv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv8i16_shift\n");
            break;
        }
        case AArch64::SSHRv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSHRv8i8_shift\n");
            break;
        }
        case AArch64::SSRAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAd\n");
            break;
        }
        case AArch64::SSRAv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv16i8_shift\n");
            break;
        }
        case AArch64::SSRAv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv2i32_shift\n");
            break;
        }
        case AArch64::SSRAv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv2i64_shift\n");
            break;
        }
        case AArch64::SSRAv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv4i16_shift\n");
            break;
        }
        case AArch64::SSRAv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv4i32_shift\n");
            break;
        }
        case AArch64::SSRAv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv8i16_shift\n");
            break;
        }
        case AArch64::SSRAv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSRAv8i8_shift\n");
            break;
        }
        case AArch64::SSUBLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBLv16i8_v8i16\n");
            break;
        }
        case AArch64::SSUBLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBLv2i32_v2i64\n");
            break;
        }
        case AArch64::SSUBLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBLv4i16_v4i32\n");
            break;
        }
        case AArch64::SSUBLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBLv4i32_v2i64\n");
            break;
        }
        case AArch64::SSUBLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBLv8i16_v4i32\n");
            break;
        }
        case AArch64::SSUBLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBLv8i8_v8i16\n");
            break;
        }
        case AArch64::SSUBWv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBWv16i8_v8i16\n");
            break;
        }
        case AArch64::SSUBWv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBWv2i32_v2i64\n");
            break;
        }
        case AArch64::SSUBWv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBWv4i16_v4i32\n");
            break;
        }
        case AArch64::SSUBWv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBWv4i32_v2i64\n");
            break;
        }
        case AArch64::SSUBWv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBWv8i16_v4i32\n");
            break;
        }
        case AArch64::SSUBWv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SSUBWv8i8_v8i16\n");
            break;
        }
        case AArch64::ST1Fourv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv16b\n");
            break;
        }
        case AArch64::ST1Fourv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv16b_POST\n");
            break;
        }
        case AArch64::ST1Fourv1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv1d\n");
            break;
        }
        case AArch64::ST1Fourv1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv1d_POST\n");
            break;
        }
        case AArch64::ST1Fourv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv2d\n");
            break;
        }
        case AArch64::ST1Fourv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv2d_POST\n");
            break;
        }
        case AArch64::ST1Fourv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv2s\n");
            break;
        }
        case AArch64::ST1Fourv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv2s_POST\n");
            break;
        }
        case AArch64::ST1Fourv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv4h\n");
            break;
        }
        case AArch64::ST1Fourv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv4h_POST\n");
            break;
        }
        case AArch64::ST1Fourv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv4s\n");
            break;
        }
        case AArch64::ST1Fourv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv4s_POST\n");
            break;
        }
        case AArch64::ST1Fourv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv8b\n");
            break;
        }
        case AArch64::ST1Fourv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv8b_POST\n");
            break;
        }
        case AArch64::ST1Fourv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv8h\n");
            break;
        }
        case AArch64::ST1Fourv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Fourv8h_POST\n");
            break;
        }
        case AArch64::ST1Onev16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev16b\n");
            break;
        }
        case AArch64::ST1Onev16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev16b_POST\n");
            break;
        }
        case AArch64::ST1Onev1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev1d\n");
            break;
        }
        case AArch64::ST1Onev1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev1d_POST\n");
            break;
        }
        case AArch64::ST1Onev2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev2d\n");
            break;
        }
        case AArch64::ST1Onev2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev2d_POST\n");
            break;
        }
        case AArch64::ST1Onev2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev2s\n");
            break;
        }
        case AArch64::ST1Onev2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev2s_POST\n");
            break;
        }
        case AArch64::ST1Onev4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev4h\n");
            break;
        }
        case AArch64::ST1Onev4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev4h_POST\n");
            break;
        }
        case AArch64::ST1Onev4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev4s\n");
            break;
        }
        case AArch64::ST1Onev4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev4s_POST\n");
            break;
        }
        case AArch64::ST1Onev8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev8b\n");
            break;
        }
        case AArch64::ST1Onev8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev8b_POST\n");
            break;
        }
        case AArch64::ST1Onev8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev8h\n");
            break;
        }
        case AArch64::ST1Onev8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Onev8h_POST\n");
            break;
        }
        case AArch64::ST1Threev16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev16b\n");
            break;
        }
        case AArch64::ST1Threev16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev16b_POST\n");
            break;
        }
        case AArch64::ST1Threev1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev1d\n");
            break;
        }
        case AArch64::ST1Threev1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev1d_POST\n");
            break;
        }
        case AArch64::ST1Threev2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev2d\n");
            break;
        }
        case AArch64::ST1Threev2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev2d_POST\n");
            break;
        }
        case AArch64::ST1Threev2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev2s\n");
            break;
        }
        case AArch64::ST1Threev2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev2s_POST\n");
            break;
        }
        case AArch64::ST1Threev4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev4h\n");
            break;
        }
        case AArch64::ST1Threev4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev4h_POST\n");
            break;
        }
        case AArch64::ST1Threev4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev4s\n");
            break;
        }
        case AArch64::ST1Threev4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev4s_POST\n");
            break;
        }
        case AArch64::ST1Threev8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev8b\n");
            break;
        }
        case AArch64::ST1Threev8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev8b_POST\n");
            break;
        }
        case AArch64::ST1Threev8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev8h\n");
            break;
        }
        case AArch64::ST1Threev8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Threev8h_POST\n");
            break;
        }
        case AArch64::ST1Twov16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov16b\n");
            break;
        }
        case AArch64::ST1Twov16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov16b_POST\n");
            break;
        }
        case AArch64::ST1Twov1d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov1d\n");
            break;
        }
        case AArch64::ST1Twov1d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov1d_POST\n");
            break;
        }
        case AArch64::ST1Twov2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov2d\n");
            break;
        }
        case AArch64::ST1Twov2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov2d_POST\n");
            break;
        }
        case AArch64::ST1Twov2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov2s\n");
            break;
        }
        case AArch64::ST1Twov2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov2s_POST\n");
            break;
        }
        case AArch64::ST1Twov4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov4h\n");
            break;
        }
        case AArch64::ST1Twov4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov4h_POST\n");
            break;
        }
        case AArch64::ST1Twov4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov4s\n");
            break;
        }
        case AArch64::ST1Twov4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov4s_POST\n");
            break;
        }
        case AArch64::ST1Twov8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov8b\n");
            break;
        }
        case AArch64::ST1Twov8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov8b_POST\n");
            break;
        }
        case AArch64::ST1Twov8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov8h\n");
            break;
        }
        case AArch64::ST1Twov8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1Twov8h_POST\n");
            break;
        }
        case AArch64::ST1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i16\n");
            break;
        }
        case AArch64::ST1i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i16_POST\n");
            break;
        }
        case AArch64::ST1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i32\n");
            break;
        }
        case AArch64::ST1i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i32_POST\n");
            break;
        }
        case AArch64::ST1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i64\n");
            break;
        }
        case AArch64::ST1i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i64_POST\n");
            break;
        }
        case AArch64::ST1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i8\n");
            break;
        }
        case AArch64::ST1i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST1i8_POST\n");
            break;
        }
        case AArch64::ST2Twov16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov16b\n");
            break;
        }
        case AArch64::ST2Twov16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov16b_POST\n");
            break;
        }
        case AArch64::ST2Twov2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov2d\n");
            break;
        }
        case AArch64::ST2Twov2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov2d_POST\n");
            break;
        }
        case AArch64::ST2Twov2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov2s\n");
            break;
        }
        case AArch64::ST2Twov2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov2s_POST\n");
            break;
        }
        case AArch64::ST2Twov4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov4h\n");
            break;
        }
        case AArch64::ST2Twov4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov4h_POST\n");
            break;
        }
        case AArch64::ST2Twov4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov4s\n");
            break;
        }
        case AArch64::ST2Twov4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov4s_POST\n");
            break;
        }
        case AArch64::ST2Twov8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov8b\n");
            break;
        }
        case AArch64::ST2Twov8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov8b_POST\n");
            break;
        }
        case AArch64::ST2Twov8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov8h\n");
            break;
        }
        case AArch64::ST2Twov8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2Twov8h_POST\n");
            break;
        }
        case AArch64::ST2i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i16\n");
            break;
        }
        case AArch64::ST2i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i16_POST\n");
            break;
        }
        case AArch64::ST2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i32\n");
            break;
        }
        case AArch64::ST2i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i32_POST\n");
            break;
        }
        case AArch64::ST2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i64\n");
            break;
        }
        case AArch64::ST2i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i64_POST\n");
            break;
        }
        case AArch64::ST2i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i8\n");
            break;
        }
        case AArch64::ST2i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST2i8_POST\n");
            break;
        }
        case AArch64::ST3Threev16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev16b\n");
            break;
        }
        case AArch64::ST3Threev16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev16b_POST\n");
            break;
        }
        case AArch64::ST3Threev2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev2d\n");
            break;
        }
        case AArch64::ST3Threev2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev2d_POST\n");
            break;
        }
        case AArch64::ST3Threev2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev2s\n");
            break;
        }
        case AArch64::ST3Threev2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev2s_POST\n");
            break;
        }
        case AArch64::ST3Threev4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev4h\n");
            break;
        }
        case AArch64::ST3Threev4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev4h_POST\n");
            break;
        }
        case AArch64::ST3Threev4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev4s\n");
            break;
        }
        case AArch64::ST3Threev4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev4s_POST\n");
            break;
        }
        case AArch64::ST3Threev8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev8b\n");
            break;
        }
        case AArch64::ST3Threev8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev8b_POST\n");
            break;
        }
        case AArch64::ST3Threev8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev8h\n");
            break;
        }
        case AArch64::ST3Threev8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3Threev8h_POST\n");
            break;
        }
        case AArch64::ST3i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i16\n");
            break;
        }
        case AArch64::ST3i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i16_POST\n");
            break;
        }
        case AArch64::ST3i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i32\n");
            break;
        }
        case AArch64::ST3i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i32_POST\n");
            break;
        }
        case AArch64::ST3i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i64\n");
            break;
        }
        case AArch64::ST3i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i64_POST\n");
            break;
        }
        case AArch64::ST3i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i8\n");
            break;
        }
        case AArch64::ST3i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST3i8_POST\n");
            break;
        }
        case AArch64::ST4Fourv16b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv16b\n");
            break;
        }
        case AArch64::ST4Fourv16b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv16b_POST\n");
            break;
        }
        case AArch64::ST4Fourv2d: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv2d\n");
            break;
        }
        case AArch64::ST4Fourv2d_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv2d_POST\n");
            break;
        }
        case AArch64::ST4Fourv2s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv2s\n");
            break;
        }
        case AArch64::ST4Fourv2s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv2s_POST\n");
            break;
        }
        case AArch64::ST4Fourv4h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv4h\n");
            break;
        }
        case AArch64::ST4Fourv4h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv4h_POST\n");
            break;
        }
        case AArch64::ST4Fourv4s: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv4s\n");
            break;
        }
        case AArch64::ST4Fourv4s_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv4s_POST\n");
            break;
        }
        case AArch64::ST4Fourv8b: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv8b\n");
            break;
        }
        case AArch64::ST4Fourv8b_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv8b_POST\n");
            break;
        }
        case AArch64::ST4Fourv8h: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv8h\n");
            break;
        }
        case AArch64::ST4Fourv8h_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4Fourv8h_POST\n");
            break;
        }
        case AArch64::ST4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i16\n");
            break;
        }
        case AArch64::ST4i16_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i16_POST\n");
            break;
        }
        case AArch64::ST4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i32\n");
            break;
        }
        case AArch64::ST4i32_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i32_POST\n");
            break;
        }
        case AArch64::ST4i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i64\n");
            break;
        }
        case AArch64::ST4i64_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i64_POST\n");
            break;
        }
        case AArch64::ST4i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i8\n");
            break;
        }
        case AArch64::ST4i8_POST: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ST4i8_POST\n");
            break;
        }
        case AArch64::STLLRB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLLRB\n");
            break;
        }
        case AArch64::STLLRH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLLRH\n");
            break;
        }
        case AArch64::STLLRW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLLRW\n");
            break;
        }
        case AArch64::STLLRX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLLRX\n");
            break;
        }
        case AArch64::STLRB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLRB\n");
            break;
        }
        case AArch64::STLRH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLRH\n");
            break;
        }
        case AArch64::STLRW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLRW\n");
            break;
        }
        case AArch64::STLRX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLRX\n");
            break;
        }
        case AArch64::STLXPW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLXPW\n");
            break;
        }
        case AArch64::STLXPX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLXPX\n");
            break;
        }
        case AArch64::STLXRB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLXRB\n");
            break;
        }
        case AArch64::STLXRH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLXRH\n");
            break;
        }
        case AArch64::STLXRW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLXRW\n");
            break;
        }
        case AArch64::STLXRX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STLXRX\n");
            break;
        }
        case AArch64::STNPDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STNPDi\n");
            break;
        }
        case AArch64::STNPQi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STNPQi\n");
            break;
        }
        case AArch64::STNPSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STNPSi\n");
            break;
        }
        case AArch64::STNPWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STNPWi\n");
            break;
        }
        case AArch64::STNPXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STNPXi\n");
            break;
        }
        case AArch64::STPDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPDi\n");
            break;
        }
        case AArch64::STPDpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPDpost\n");
            break;
        }
        case AArch64::STPDpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPDpre\n");
            break;
        }
        case AArch64::STPQi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPQi\n");
            break;
        }
        case AArch64::STPQpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPQpost\n");
            break;
        }
        case AArch64::STPQpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPQpre\n");
            break;
        }
        case AArch64::STPSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPSi\n");
            break;
        }
        case AArch64::STPSpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPSpost\n");
            break;
        }
        case AArch64::STPSpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPSpre\n");
            break;
        }
        case AArch64::STPWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPWi\n");
            break;
        }
        case AArch64::STPWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPWpost\n");
            break;
        }
        case AArch64::STPWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPWpre\n");
            break;
        }
        case AArch64::STPXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPXi\n");
            break;
        }
        case AArch64::STPXpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPXpost\n");
            break;
        }
        case AArch64::STPXpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STPXpre\n");
            break;
        }
        case AArch64::STRBBpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBBpost\n");
            break;
        }
        case AArch64::STRBBpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBBpre\n");
            break;
        }
        case AArch64::STRBBroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBBroW\n");
            break;
        }
        case AArch64::STRBBroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBBroX\n");
            break;
        }
        case AArch64::STRBBui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBBui\n");
            break;
        }
        case AArch64::STRBpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBpost\n");
            break;
        }
        case AArch64::STRBpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBpre\n");
            break;
        }
        case AArch64::STRBroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBroW\n");
            break;
        }
        case AArch64::STRBroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBroX\n");
            break;
        }
        case AArch64::STRBui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRBui\n");
            break;
        }
        case AArch64::STRDpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRDpost\n");
            break;
        }
        case AArch64::STRDpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRDpre\n");
            break;
        }
        case AArch64::STRDroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRDroW\n");
            break;
        }
        case AArch64::STRDroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRDroX\n");
            break;
        }
        case AArch64::STRDui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRDui\n");
            break;
        }
        case AArch64::STRHHpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHHpost\n");
            break;
        }
        case AArch64::STRHHpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHHpre\n");
            break;
        }
        case AArch64::STRHHroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHHroW\n");
            break;
        }
        case AArch64::STRHHroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHHroX\n");
            break;
        }
        case AArch64::STRHHui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHHui\n");
            break;
        }
        case AArch64::STRHpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHpost\n");
            break;
        }
        case AArch64::STRHpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHpre\n");
            break;
        }
        case AArch64::STRHroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHroW\n");
            break;
        }
        case AArch64::STRHroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHroX\n");
            break;
        }
        case AArch64::STRHui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRHui\n");
            break;
        }
        case AArch64::STRQpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRQpost\n");
            break;
        }
        case AArch64::STRQpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRQpre\n");
            break;
        }
        case AArch64::STRQroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRQroW\n");
            break;
        }
        case AArch64::STRQroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRQroX\n");
            break;
        }
        case AArch64::STRQui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRQui\n");
            break;
        }
        case AArch64::STRSpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRSpost\n");
            break;
        }
        case AArch64::STRSpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRSpre\n");
            break;
        }
        case AArch64::STRSroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRSroW\n");
            break;
        }
        case AArch64::STRSroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRSroX\n");
            break;
        }
        case AArch64::STRSui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRSui\n");
            break;
        }
        case AArch64::STRWpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRWpost\n");
            break;
        }
        case AArch64::STRWpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRWpre\n");
            break;
        }
        case AArch64::STRWroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRWroW\n");
            break;
        }
        case AArch64::STRWroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRWroX\n");
            break;
        }
        case AArch64::STRWui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRWui\n");
            break;
        }
        case AArch64::STRXpost: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRXpost\n");
            break;
        }
        case AArch64::STRXpre: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRXpre\n");
            break;
        }
        case AArch64::STRXroW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRXroW\n");
            break;
        }
        case AArch64::STRXroX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRXroX\n");
            break;
        }
        case AArch64::STRXui: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STRXui\n");
            break;
        }
        case AArch64::STTRBi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STTRBi\n");
            break;
        }
        case AArch64::STTRHi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STTRHi\n");
            break;
        }
        case AArch64::STTRWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STTRWi\n");
            break;
        }
        case AArch64::STTRXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STTRXi\n");
            break;
        }
        case AArch64::STURBBi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURBBi\n");
            break;
        }
        case AArch64::STURBi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURBi\n");
            break;
        }
        case AArch64::STURDi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURDi\n");
            break;
        }
        case AArch64::STURHHi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURHHi\n");
            break;
        }
        case AArch64::STURHi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURHi\n");
            break;
        }
        case AArch64::STURQi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURQi\n");
            break;
        }
        case AArch64::STURSi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURSi\n");
            break;
        }
        case AArch64::STURWi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURWi\n");
            break;
        }
        case AArch64::STURXi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STURXi\n");
            break;
        }
        case AArch64::STXPW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STXPW\n");
            break;
        }
        case AArch64::STXPX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STXPX\n");
            break;
        }
        case AArch64::STXRB: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STXRB\n");
            break;
        }
        case AArch64::STXRH: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STXRH\n");
            break;
        }
        case AArch64::STXRW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STXRW\n");
            break;
        }
        case AArch64::STXRX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " STXRX\n");
            break;
        }
        case AArch64::SUBHNv2i64_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBHNv2i64_v2i32\n");
            break;
        }
        case AArch64::SUBHNv2i64_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBHNv2i64_v4i32\n");
            break;
        }
        case AArch64::SUBHNv4i32_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBHNv4i32_v4i16\n");
            break;
        }
        case AArch64::SUBHNv4i32_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBHNv4i32_v8i16\n");
            break;
        }
        case AArch64::SUBHNv8i16_v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBHNv8i16_v16i8\n");
            break;
        }
        case AArch64::SUBHNv8i16_v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBHNv8i16_v8i8\n");
            break;
        }
        case AArch64::SUBSWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSWri\n");
            break;
        }
        case AArch64::SUBSWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSWrr\n");
            break;
        }
        case AArch64::SUBSWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSWrs\n");
            break;
        }
        case AArch64::SUBSWrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSWrx\n");
            break;
        }
        case AArch64::SUBSXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSXri\n");
            break;
        }
        case AArch64::SUBSXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSXrr\n");
            break;
        }
        case AArch64::SUBSXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSXrs\n");
            break;
        }
        case AArch64::SUBSXrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSXrx\n");
            break;
        }
        case AArch64::SUBSXrx64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBSXrx64\n");
            break;
        }
        case AArch64::SUBWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBWri\n");
            break;
        }
        case AArch64::SUBWrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBWrr\n");
            break;
        }
        case AArch64::SUBWrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBWrs\n");
            break;
        }
        case AArch64::SUBWrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBWrx\n");
            break;
        }
        case AArch64::SUBXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBXri\n");
            break;
        }
        case AArch64::SUBXrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBXrr\n");
            break;
        }
        case AArch64::SUBXrs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBXrs\n");
            break;
        }
        case AArch64::SUBXrx: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBXrx\n");
            break;
        }
        case AArch64::SUBXrx64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBXrx64\n");
            break;
        }
        case AArch64::SUBv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv16i8\n");
            break;
        }
        case AArch64::SUBv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv1i64\n");
            break;
        }
        case AArch64::SUBv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv2i32\n");
            break;
        }
        case AArch64::SUBv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv2i64\n");
            break;
        }
        case AArch64::SUBv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv4i16\n");
            break;
        }
        case AArch64::SUBv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv4i32\n");
            break;
        }
        case AArch64::SUBv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv8i16\n");
            break;
        }
        case AArch64::SUBv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUBv8i8\n");
            break;
        }
        case AArch64::SUQADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv16i8\n");
            break;
        }
        case AArch64::SUQADDv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv1i16\n");
            break;
        }
        case AArch64::SUQADDv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv1i32\n");
            break;
        }
        case AArch64::SUQADDv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv1i64\n");
            break;
        }
        case AArch64::SUQADDv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv1i8\n");
            break;
        }
        case AArch64::SUQADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv2i32\n");
            break;
        }
        case AArch64::SUQADDv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv2i64\n");
            break;
        }
        case AArch64::SUQADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv4i16\n");
            break;
        }
        case AArch64::SUQADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv4i32\n");
            break;
        }
        case AArch64::SUQADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv8i16\n");
            break;
        }
        case AArch64::SUQADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SUQADDv8i8\n");
            break;
        }
        case AArch64::SVC: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SVC\n");
            break;
        }
        case AArch64::SWPALb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPALb\n");
            break;
        }
        case AArch64::SWPALd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPALd\n");
            break;
        }
        case AArch64::SWPALh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPALh\n");
            break;
        }
        case AArch64::SWPALs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPALs\n");
            break;
        }
        case AArch64::SWPAb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPAb\n");
            break;
        }
        case AArch64::SWPAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPAd\n");
            break;
        }
        case AArch64::SWPAh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPAh\n");
            break;
        }
        case AArch64::SWPAs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPAs\n");
            break;
        }
        case AArch64::SWPLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPLb\n");
            break;
        }
        case AArch64::SWPLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPLd\n");
            break;
        }
        case AArch64::SWPLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPLh\n");
            break;
        }
        case AArch64::SWPLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPLs\n");
            break;
        }
        case AArch64::SWPb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPb\n");
            break;
        }
        case AArch64::SWPd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPd\n");
            break;
        }
        case AArch64::SWPh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPh\n");
            break;
        }
        case AArch64::SWPs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SWPs\n");
            break;
        }
        case AArch64::SYSLxt: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SYSLxt\n");
            break;
        }
        case AArch64::SYSxt: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " SYSxt\n");
            break;
        }
        case AArch64::TBLv16i8Four: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv16i8Four\n");
            break;
        }
        case AArch64::TBLv16i8One: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv16i8One\n");
            break;
        }
        case AArch64::TBLv16i8Three: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv16i8Three\n");
            break;
        }
        case AArch64::TBLv16i8Two: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv16i8Two\n");
            break;
        }
        case AArch64::TBLv8i8Four: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv8i8Four\n");
            break;
        }
        case AArch64::TBLv8i8One: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv8i8One\n");
            break;
        }
        case AArch64::TBLv8i8Three: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv8i8Three\n");
            break;
        }
        case AArch64::TBLv8i8Two: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBLv8i8Two\n");
            break;
        }
        case AArch64::TBNZW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBNZW\n");
            break;
        }
        case AArch64::TBNZX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBNZX\n");
            break;
        }
        case AArch64::TBXv16i8Four: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv16i8Four\n");
            break;
        }
        case AArch64::TBXv16i8One: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv16i8One\n");
            break;
        }
        case AArch64::TBXv16i8Three: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv16i8Three\n");
            break;
        }
        case AArch64::TBXv16i8Two: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv16i8Two\n");
            break;
        }
        case AArch64::TBXv8i8Four: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv8i8Four\n");
            break;
        }
        case AArch64::TBXv8i8One: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv8i8One\n");
            break;
        }
        case AArch64::TBXv8i8Three: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv8i8Three\n");
            break;
        }
        case AArch64::TBXv8i8Two: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBXv8i8Two\n");
            break;
        }
        case AArch64::TBZW: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBZW\n");
            break;
        }
        case AArch64::TBZX: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TBZX\n");
            break;
        }
        case AArch64::TCRETURNdi: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TCRETURNdi\n");
            break;
        }
        case AArch64::TCRETURNri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TCRETURNri\n");
            break;
        }
        case AArch64::TLSDESCCALL: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TLSDESCCALL\n");
            break;
        }
        case AArch64::TLSDESC_CALLSEQ: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TLSDESC_CALLSEQ\n");
            break;
        }
        case AArch64::TRN1v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v16i8\n");
            break;
        }
        case AArch64::TRN1v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v2i32\n");
            break;
        }
        case AArch64::TRN1v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v2i64\n");
            break;
        }
        case AArch64::TRN1v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v4i16\n");
            break;
        }
        case AArch64::TRN1v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v4i32\n");
            break;
        }
        case AArch64::TRN1v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v8i16\n");
            break;
        }
        case AArch64::TRN1v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN1v8i8\n");
            break;
        }
        case AArch64::TRN2v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v16i8\n");
            break;
        }
        case AArch64::TRN2v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v2i32\n");
            break;
        }
        case AArch64::TRN2v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v2i64\n");
            break;
        }
        case AArch64::TRN2v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v4i16\n");
            break;
        }
        case AArch64::TRN2v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v4i32\n");
            break;
        }
        case AArch64::TRN2v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v8i16\n");
            break;
        }
        case AArch64::TRN2v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " TRN2v8i8\n");
            break;
        }
        case AArch64::UABALv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABALv16i8_v8i16\n");
            break;
        }
        case AArch64::UABALv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABALv2i32_v2i64\n");
            break;
        }
        case AArch64::UABALv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABALv4i16_v4i32\n");
            break;
        }
        case AArch64::UABALv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABALv4i32_v2i64\n");
            break;
        }
        case AArch64::UABALv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABALv8i16_v4i32\n");
            break;
        }
        case AArch64::UABALv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABALv8i8_v8i16\n");
            break;
        }
        case AArch64::UABAv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABAv16i8\n");
            break;
        }
        case AArch64::UABAv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABAv2i32\n");
            break;
        }
        case AArch64::UABAv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABAv4i16\n");
            break;
        }
        case AArch64::UABAv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABAv4i32\n");
            break;
        }
        case AArch64::UABAv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABAv8i16\n");
            break;
        }
        case AArch64::UABAv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABAv8i8\n");
            break;
        }
        case AArch64::UABDLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDLv16i8_v8i16\n");
            break;
        }
        case AArch64::UABDLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDLv2i32_v2i64\n");
            break;
        }
        case AArch64::UABDLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDLv4i16_v4i32\n");
            break;
        }
        case AArch64::UABDLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDLv4i32_v2i64\n");
            break;
        }
        case AArch64::UABDLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDLv8i16_v4i32\n");
            break;
        }
        case AArch64::UABDLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDLv8i8_v8i16\n");
            break;
        }
        case AArch64::UABDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDv16i8\n");
            break;
        }
        case AArch64::UABDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDv2i32\n");
            break;
        }
        case AArch64::UABDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDv4i16\n");
            break;
        }
        case AArch64::UABDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDv4i32\n");
            break;
        }
        case AArch64::UABDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDv8i16\n");
            break;
        }
        case AArch64::UABDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UABDv8i8\n");
            break;
        }
        case AArch64::UADALPv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADALPv16i8_v8i16\n");
            break;
        }
        case AArch64::UADALPv2i32_v1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADALPv2i32_v1i64\n");
            break;
        }
        case AArch64::UADALPv4i16_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADALPv4i16_v2i32\n");
            break;
        }
        case AArch64::UADALPv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADALPv4i32_v2i64\n");
            break;
        }
        case AArch64::UADALPv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADALPv8i16_v4i32\n");
            break;
        }
        case AArch64::UADALPv8i8_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADALPv8i8_v4i16\n");
            break;
        }
        case AArch64::UADDLPv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLPv16i8_v8i16\n");
            break;
        }
        case AArch64::UADDLPv2i32_v1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLPv2i32_v1i64\n");
            break;
        }
        case AArch64::UADDLPv4i16_v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLPv4i16_v2i32\n");
            break;
        }
        case AArch64::UADDLPv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLPv4i32_v2i64\n");
            break;
        }
        case AArch64::UADDLPv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLPv8i16_v4i32\n");
            break;
        }
        case AArch64::UADDLPv8i8_v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLPv8i8_v4i16\n");
            break;
        }
        case AArch64::UADDLVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLVv16i8v\n");
            break;
        }
        case AArch64::UADDLVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLVv4i16v\n");
            break;
        }
        case AArch64::UADDLVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLVv4i32v\n");
            break;
        }
        case AArch64::UADDLVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLVv8i16v\n");
            break;
        }
        case AArch64::UADDLVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLVv8i8v\n");
            break;
        }
        case AArch64::UADDLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLv16i8_v8i16\n");
            break;
        }
        case AArch64::UADDLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLv2i32_v2i64\n");
            break;
        }
        case AArch64::UADDLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLv4i16_v4i32\n");
            break;
        }
        case AArch64::UADDLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLv4i32_v2i64\n");
            break;
        }
        case AArch64::UADDLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLv8i16_v4i32\n");
            break;
        }
        case AArch64::UADDLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDLv8i8_v8i16\n");
            break;
        }
        case AArch64::UADDWv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDWv16i8_v8i16\n");
            break;
        }
        case AArch64::UADDWv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDWv2i32_v2i64\n");
            break;
        }
        case AArch64::UADDWv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDWv4i16_v4i32\n");
            break;
        }
        case AArch64::UADDWv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDWv4i32_v2i64\n");
            break;
        }
        case AArch64::UADDWv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDWv8i16_v4i32\n");
            break;
        }
        case AArch64::UADDWv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UADDWv8i8_v8i16\n");
            break;
        }
        case AArch64::UBFMWri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UBFMWri\n");
            break;
        }
        case AArch64::UBFMXri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UBFMXri\n");
            break;
        }
        case AArch64::UCVTFSWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFSWDri\n");
            break;
        }
        case AArch64::UCVTFSWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFSWSri\n");
            break;
        }
        case AArch64::UCVTFSXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFSXDri\n");
            break;
        }
        case AArch64::UCVTFSXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFSXSri\n");
            break;
        }
        case AArch64::UCVTFUWDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFUWDri\n");
            break;
        }
        case AArch64::UCVTFUWSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFUWSri\n");
            break;
        }
        case AArch64::UCVTFUXDri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFUXDri\n");
            break;
        }
        case AArch64::UCVTFUXSri: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFUXSri\n");
            break;
        }
        case AArch64::UCVTFd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFd\n");
            break;
        }
        case AArch64::UCVTFs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFs\n");
            break;
        }
        case AArch64::UCVTFv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv1i32\n");
            break;
        }
        case AArch64::UCVTFv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv1i64\n");
            break;
        }
        case AArch64::UCVTFv2f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv2f32\n");
            break;
        }
        case AArch64::UCVTFv2f64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv2f64\n");
            break;
        }
        case AArch64::UCVTFv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv2i32_shift\n");
            break;
        }
        case AArch64::UCVTFv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv2i64_shift\n");
            break;
        }
        case AArch64::UCVTFv4f32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv4f32\n");
            break;
        }
        case AArch64::UCVTFv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UCVTFv4i32_shift\n");
            break;
        }
        case AArch64::UDIVWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UDIVWr\n");
            break;
        }
        case AArch64::UDIVXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UDIVXr\n");
            break;
        }
        case AArch64::UDIV_IntWr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UDIV_IntWr\n");
            break;
        }
        case AArch64::UDIV_IntXr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UDIV_IntXr\n");
            break;
        }
        case AArch64::UHADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHADDv16i8\n");
            break;
        }
        case AArch64::UHADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHADDv2i32\n");
            break;
        }
        case AArch64::UHADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHADDv4i16\n");
            break;
        }
        case AArch64::UHADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHADDv4i32\n");
            break;
        }
        case AArch64::UHADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHADDv8i16\n");
            break;
        }
        case AArch64::UHADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHADDv8i8\n");
            break;
        }
        case AArch64::UHSUBv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHSUBv16i8\n");
            break;
        }
        case AArch64::UHSUBv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHSUBv2i32\n");
            break;
        }
        case AArch64::UHSUBv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHSUBv4i16\n");
            break;
        }
        case AArch64::UHSUBv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHSUBv4i32\n");
            break;
        }
        case AArch64::UHSUBv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHSUBv8i16\n");
            break;
        }
        case AArch64::UHSUBv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UHSUBv8i8\n");
            break;
        }
        case AArch64::UMADDLrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMADDLrrr\n");
            break;
        }
        case AArch64::UMAXPv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXPv16i8\n");
            break;
        }
        case AArch64::UMAXPv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXPv2i32\n");
            break;
        }
        case AArch64::UMAXPv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXPv4i16\n");
            break;
        }
        case AArch64::UMAXPv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXPv4i32\n");
            break;
        }
        case AArch64::UMAXPv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXPv8i16\n");
            break;
        }
        case AArch64::UMAXPv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXPv8i8\n");
            break;
        }
        case AArch64::UMAXVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXVv16i8v\n");
            break;
        }
        case AArch64::UMAXVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXVv4i16v\n");
            break;
        }
        case AArch64::UMAXVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXVv4i32v\n");
            break;
        }
        case AArch64::UMAXVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXVv8i16v\n");
            break;
        }
        case AArch64::UMAXVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXVv8i8v\n");
            break;
        }
        case AArch64::UMAXv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXv16i8\n");
            break;
        }
        case AArch64::UMAXv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXv2i32\n");
            break;
        }
        case AArch64::UMAXv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXv4i16\n");
            break;
        }
        case AArch64::UMAXv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXv4i32\n");
            break;
        }
        case AArch64::UMAXv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXv8i16\n");
            break;
        }
        case AArch64::UMAXv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMAXv8i8\n");
            break;
        }
        case AArch64::UMINPv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINPv16i8\n");
            break;
        }
        case AArch64::UMINPv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINPv2i32\n");
            break;
        }
        case AArch64::UMINPv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINPv4i16\n");
            break;
        }
        case AArch64::UMINPv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINPv4i32\n");
            break;
        }
        case AArch64::UMINPv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINPv8i16\n");
            break;
        }
        case AArch64::UMINPv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINPv8i8\n");
            break;
        }
        case AArch64::UMINVv16i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINVv16i8v\n");
            break;
        }
        case AArch64::UMINVv4i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINVv4i16v\n");
            break;
        }
        case AArch64::UMINVv4i32v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINVv4i32v\n");
            break;
        }
        case AArch64::UMINVv8i16v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINVv8i16v\n");
            break;
        }
        case AArch64::UMINVv8i8v: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINVv8i8v\n");
            break;
        }
        case AArch64::UMINv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINv16i8\n");
            break;
        }
        case AArch64::UMINv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINv2i32\n");
            break;
        }
        case AArch64::UMINv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINv4i16\n");
            break;
        }
        case AArch64::UMINv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINv4i32\n");
            break;
        }
        case AArch64::UMINv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINv8i16\n");
            break;
        }
        case AArch64::UMINv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMINv8i8\n");
            break;
        }
        case AArch64::UMLALv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv16i8_v8i16\n");
            break;
        }
        case AArch64::UMLALv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv2i32_indexed\n");
            break;
        }
        case AArch64::UMLALv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv2i32_v2i64\n");
            break;
        }
        case AArch64::UMLALv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv4i16_indexed\n");
            break;
        }
        case AArch64::UMLALv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv4i16_v4i32\n");
            break;
        }
        case AArch64::UMLALv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv4i32_indexed\n");
            break;
        }
        case AArch64::UMLALv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv4i32_v2i64\n");
            break;
        }
        case AArch64::UMLALv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv8i16_indexed\n");
            break;
        }
        case AArch64::UMLALv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv8i16_v4i32\n");
            break;
        }
        case AArch64::UMLALv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLALv8i8_v8i16\n");
            break;
        }
        case AArch64::UMLSLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv16i8_v8i16\n");
            break;
        }
        case AArch64::UMLSLv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv2i32_indexed\n");
            break;
        }
        case AArch64::UMLSLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv2i32_v2i64\n");
            break;
        }
        case AArch64::UMLSLv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv4i16_indexed\n");
            break;
        }
        case AArch64::UMLSLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv4i16_v4i32\n");
            break;
        }
        case AArch64::UMLSLv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv4i32_indexed\n");
            break;
        }
        case AArch64::UMLSLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv4i32_v2i64\n");
            break;
        }
        case AArch64::UMLSLv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv8i16_indexed\n");
            break;
        }
        case AArch64::UMLSLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv8i16_v4i32\n");
            break;
        }
        case AArch64::UMLSLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMLSLv8i8_v8i16\n");
            break;
        }
        case AArch64::UMOVvi16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMOVvi16\n");
            break;
        }
        case AArch64::UMOVvi32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMOVvi32\n");
            break;
        }
        case AArch64::UMOVvi64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMOVvi64\n");
            break;
        }
        case AArch64::UMOVvi8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMOVvi8\n");
            break;
        }
        case AArch64::UMSUBLrrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMSUBLrrr\n");
            break;
        }
        case AArch64::UMULHrr: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULHrr\n");
            break;
        }
        case AArch64::UMULLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv16i8_v8i16\n");
            break;
        }
        case AArch64::UMULLv2i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv2i32_indexed\n");
            break;
        }
        case AArch64::UMULLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv2i32_v2i64\n");
            break;
        }
        case AArch64::UMULLv4i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv4i16_indexed\n");
            break;
        }
        case AArch64::UMULLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv4i16_v4i32\n");
            break;
        }
        case AArch64::UMULLv4i32_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv4i32_indexed\n");
            break;
        }
        case AArch64::UMULLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv4i32_v2i64\n");
            break;
        }
        case AArch64::UMULLv8i16_indexed: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv8i16_indexed\n");
            break;
        }
        case AArch64::UMULLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv8i16_v4i32\n");
            break;
        }
        case AArch64::UMULLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UMULLv8i8_v8i16\n");
            break;
        }
        case AArch64::UQADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv16i8\n");
            break;
        }
        case AArch64::UQADDv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv1i16\n");
            break;
        }
        case AArch64::UQADDv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv1i32\n");
            break;
        }
        case AArch64::UQADDv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv1i64\n");
            break;
        }
        case AArch64::UQADDv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv1i8\n");
            break;
        }
        case AArch64::UQADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv2i32\n");
            break;
        }
        case AArch64::UQADDv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv2i64\n");
            break;
        }
        case AArch64::UQADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv4i16\n");
            break;
        }
        case AArch64::UQADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv4i32\n");
            break;
        }
        case AArch64::UQADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv8i16\n");
            break;
        }
        case AArch64::UQADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQADDv8i8\n");
            break;
        }
        case AArch64::UQRSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv16i8\n");
            break;
        }
        case AArch64::UQRSHLv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv1i16\n");
            break;
        }
        case AArch64::UQRSHLv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv1i32\n");
            break;
        }
        case AArch64::UQRSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv1i64\n");
            break;
        }
        case AArch64::UQRSHLv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv1i8\n");
            break;
        }
        case AArch64::UQRSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv2i32\n");
            break;
        }
        case AArch64::UQRSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv2i64\n");
            break;
        }
        case AArch64::UQRSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv4i16\n");
            break;
        }
        case AArch64::UQRSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv4i32\n");
            break;
        }
        case AArch64::UQRSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv8i16\n");
            break;
        }
        case AArch64::UQRSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHLv8i8\n");
            break;
        }
        case AArch64::UQRSHRNb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNb\n");
            break;
        }
        case AArch64::UQRSHRNh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNh\n");
            break;
        }
        case AArch64::UQRSHRNs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNs\n");
            break;
        }
        case AArch64::UQRSHRNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNv16i8_shift\n");
            break;
        }
        case AArch64::UQRSHRNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNv2i32_shift\n");
            break;
        }
        case AArch64::UQRSHRNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNv4i16_shift\n");
            break;
        }
        case AArch64::UQRSHRNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNv4i32_shift\n");
            break;
        }
        case AArch64::UQRSHRNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNv8i16_shift\n");
            break;
        }
        case AArch64::UQRSHRNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQRSHRNv8i8_shift\n");
            break;
        }
        case AArch64::UQSHLb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLb\n");
            break;
        }
        case AArch64::UQSHLd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLd\n");
            break;
        }
        case AArch64::UQSHLh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLh\n");
            break;
        }
        case AArch64::UQSHLs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLs\n");
            break;
        }
        case AArch64::UQSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv16i8\n");
            break;
        }
        case AArch64::UQSHLv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv16i8_shift\n");
            break;
        }
        case AArch64::UQSHLv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv1i16\n");
            break;
        }
        case AArch64::UQSHLv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv1i32\n");
            break;
        }
        case AArch64::UQSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv1i64\n");
            break;
        }
        case AArch64::UQSHLv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv1i8\n");
            break;
        }
        case AArch64::UQSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv2i32\n");
            break;
        }
        case AArch64::UQSHLv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv2i32_shift\n");
            break;
        }
        case AArch64::UQSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv2i64\n");
            break;
        }
        case AArch64::UQSHLv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv2i64_shift\n");
            break;
        }
        case AArch64::UQSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv4i16\n");
            break;
        }
        case AArch64::UQSHLv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv4i16_shift\n");
            break;
        }
        case AArch64::UQSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv4i32\n");
            break;
        }
        case AArch64::UQSHLv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv4i32_shift\n");
            break;
        }
        case AArch64::UQSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv8i16\n");
            break;
        }
        case AArch64::UQSHLv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv8i16_shift\n");
            break;
        }
        case AArch64::UQSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv8i8\n");
            break;
        }
        case AArch64::UQSHLv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHLv8i8_shift\n");
            break;
        }
        case AArch64::UQSHRNb: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNb\n");
            break;
        }
        case AArch64::UQSHRNh: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNh\n");
            break;
        }
        case AArch64::UQSHRNs: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNs\n");
            break;
        }
        case AArch64::UQSHRNv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNv16i8_shift\n");
            break;
        }
        case AArch64::UQSHRNv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNv2i32_shift\n");
            break;
        }
        case AArch64::UQSHRNv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNv4i16_shift\n");
            break;
        }
        case AArch64::UQSHRNv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNv4i32_shift\n");
            break;
        }
        case AArch64::UQSHRNv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNv8i16_shift\n");
            break;
        }
        case AArch64::UQSHRNv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSHRNv8i8_shift\n");
            break;
        }
        case AArch64::UQSUBv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv16i8\n");
            break;
        }
        case AArch64::UQSUBv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv1i16\n");
            break;
        }
        case AArch64::UQSUBv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv1i32\n");
            break;
        }
        case AArch64::UQSUBv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv1i64\n");
            break;
        }
        case AArch64::UQSUBv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv1i8\n");
            break;
        }
        case AArch64::UQSUBv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv2i32\n");
            break;
        }
        case AArch64::UQSUBv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv2i64\n");
            break;
        }
        case AArch64::UQSUBv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv4i16\n");
            break;
        }
        case AArch64::UQSUBv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv4i32\n");
            break;
        }
        case AArch64::UQSUBv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv8i16\n");
            break;
        }
        case AArch64::UQSUBv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQSUBv8i8\n");
            break;
        }
        case AArch64::UQXTNv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv16i8\n");
            break;
        }
        case AArch64::UQXTNv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv1i16\n");
            break;
        }
        case AArch64::UQXTNv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv1i32\n");
            break;
        }
        case AArch64::UQXTNv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv1i8\n");
            break;
        }
        case AArch64::UQXTNv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv2i32\n");
            break;
        }
        case AArch64::UQXTNv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv4i16\n");
            break;
        }
        case AArch64::UQXTNv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv4i32\n");
            break;
        }
        case AArch64::UQXTNv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv8i16\n");
            break;
        }
        case AArch64::UQXTNv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UQXTNv8i8\n");
            break;
        }
        case AArch64::URECPEv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URECPEv2i32\n");
            break;
        }
        case AArch64::URECPEv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URECPEv4i32\n");
            break;
        }
        case AArch64::URHADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URHADDv16i8\n");
            break;
        }
        case AArch64::URHADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URHADDv2i32\n");
            break;
        }
        case AArch64::URHADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URHADDv4i16\n");
            break;
        }
        case AArch64::URHADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URHADDv4i32\n");
            break;
        }
        case AArch64::URHADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URHADDv8i16\n");
            break;
        }
        case AArch64::URHADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URHADDv8i8\n");
            break;
        }
        case AArch64::URSHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv16i8\n");
            break;
        }
        case AArch64::URSHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv1i64\n");
            break;
        }
        case AArch64::URSHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv2i32\n");
            break;
        }
        case AArch64::URSHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv2i64\n");
            break;
        }
        case AArch64::URSHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv4i16\n");
            break;
        }
        case AArch64::URSHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv4i32\n");
            break;
        }
        case AArch64::URSHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv8i16\n");
            break;
        }
        case AArch64::URSHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHLv8i8\n");
            break;
        }
        case AArch64::URSHRd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRd\n");
            break;
        }
        case AArch64::URSHRv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv16i8_shift\n");
            break;
        }
        case AArch64::URSHRv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv2i32_shift\n");
            break;
        }
        case AArch64::URSHRv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv2i64_shift\n");
            break;
        }
        case AArch64::URSHRv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv4i16_shift\n");
            break;
        }
        case AArch64::URSHRv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv4i32_shift\n");
            break;
        }
        case AArch64::URSHRv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv8i16_shift\n");
            break;
        }
        case AArch64::URSHRv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSHRv8i8_shift\n");
            break;
        }
        case AArch64::URSQRTEv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSQRTEv2i32\n");
            break;
        }
        case AArch64::URSQRTEv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSQRTEv4i32\n");
            break;
        }
        case AArch64::URSRAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAd\n");
            break;
        }
        case AArch64::URSRAv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv16i8_shift\n");
            break;
        }
        case AArch64::URSRAv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv2i32_shift\n");
            break;
        }
        case AArch64::URSRAv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv2i64_shift\n");
            break;
        }
        case AArch64::URSRAv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv4i16_shift\n");
            break;
        }
        case AArch64::URSRAv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv4i32_shift\n");
            break;
        }
        case AArch64::URSRAv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv8i16_shift\n");
            break;
        }
        case AArch64::URSRAv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " URSRAv8i8_shift\n");
            break;
        }
        case AArch64::USHLLv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLLv16i8_shift\n");
            break;
        }
        case AArch64::USHLLv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLLv2i32_shift\n");
            break;
        }
        case AArch64::USHLLv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLLv4i16_shift\n");
            break;
        }
        case AArch64::USHLLv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLLv4i32_shift\n");
            break;
        }
        case AArch64::USHLLv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLLv8i16_shift\n");
            break;
        }
        case AArch64::USHLLv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLLv8i8_shift\n");
            break;
        }
        case AArch64::USHLv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv16i8\n");
            break;
        }
        case AArch64::USHLv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv1i64\n");
            break;
        }
        case AArch64::USHLv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv2i32\n");
            break;
        }
        case AArch64::USHLv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv2i64\n");
            break;
        }
        case AArch64::USHLv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv4i16\n");
            break;
        }
        case AArch64::USHLv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv4i32\n");
            break;
        }
        case AArch64::USHLv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv8i16\n");
            break;
        }
        case AArch64::USHLv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHLv8i8\n");
            break;
        }
        case AArch64::USHRd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRd\n");
            break;
        }
        case AArch64::USHRv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv16i8_shift\n");
            break;
        }
        case AArch64::USHRv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv2i32_shift\n");
            break;
        }
        case AArch64::USHRv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv2i64_shift\n");
            break;
        }
        case AArch64::USHRv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv4i16_shift\n");
            break;
        }
        case AArch64::USHRv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv4i32_shift\n");
            break;
        }
        case AArch64::USHRv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv8i16_shift\n");
            break;
        }
        case AArch64::USHRv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USHRv8i8_shift\n");
            break;
        }
        case AArch64::USQADDv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv16i8\n");
            break;
        }
        case AArch64::USQADDv1i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv1i16\n");
            break;
        }
        case AArch64::USQADDv1i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv1i32\n");
            break;
        }
        case AArch64::USQADDv1i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv1i64\n");
            break;
        }
        case AArch64::USQADDv1i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv1i8\n");
            break;
        }
        case AArch64::USQADDv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv2i32\n");
            break;
        }
        case AArch64::USQADDv2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv2i64\n");
            break;
        }
        case AArch64::USQADDv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv4i16\n");
            break;
        }
        case AArch64::USQADDv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv4i32\n");
            break;
        }
        case AArch64::USQADDv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv8i16\n");
            break;
        }
        case AArch64::USQADDv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USQADDv8i8\n");
            break;
        }
        case AArch64::USRAd: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAd\n");
            break;
        }
        case AArch64::USRAv16i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv16i8_shift\n");
            break;
        }
        case AArch64::USRAv2i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv2i32_shift\n");
            break;
        }
        case AArch64::USRAv2i64_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv2i64_shift\n");
            break;
        }
        case AArch64::USRAv4i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv4i16_shift\n");
            break;
        }
        case AArch64::USRAv4i32_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv4i32_shift\n");
            break;
        }
        case AArch64::USRAv8i16_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv8i16_shift\n");
            break;
        }
        case AArch64::USRAv8i8_shift: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USRAv8i8_shift\n");
            break;
        }
        case AArch64::USUBLv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBLv16i8_v8i16\n");
            break;
        }
        case AArch64::USUBLv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBLv2i32_v2i64\n");
            break;
        }
        case AArch64::USUBLv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBLv4i16_v4i32\n");
            break;
        }
        case AArch64::USUBLv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBLv4i32_v2i64\n");
            break;
        }
        case AArch64::USUBLv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBLv8i16_v4i32\n");
            break;
        }
        case AArch64::USUBLv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBLv8i8_v8i16\n");
            break;
        }
        case AArch64::USUBWv16i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBWv16i8_v8i16\n");
            break;
        }
        case AArch64::USUBWv2i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBWv2i32_v2i64\n");
            break;
        }
        case AArch64::USUBWv4i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBWv4i16_v4i32\n");
            break;
        }
        case AArch64::USUBWv4i32_v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBWv4i32_v2i64\n");
            break;
        }
        case AArch64::USUBWv8i16_v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBWv8i16_v4i32\n");
            break;
        }
        case AArch64::USUBWv8i8_v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " USUBWv8i8_v8i16\n");
            break;
        }
        case AArch64::UZP1v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v16i8\n");
            break;
        }
        case AArch64::UZP1v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v2i32\n");
            break;
        }
        case AArch64::UZP1v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v2i64\n");
            break;
        }
        case AArch64::UZP1v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v4i16\n");
            break;
        }
        case AArch64::UZP1v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v4i32\n");
            break;
        }
        case AArch64::UZP1v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v8i16\n");
            break;
        }
        case AArch64::UZP1v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP1v8i8\n");
            break;
        }
        case AArch64::UZP2v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v16i8\n");
            break;
        }
        case AArch64::UZP2v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v2i32\n");
            break;
        }
        case AArch64::UZP2v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v2i64\n");
            break;
        }
        case AArch64::UZP2v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v4i16\n");
            break;
        }
        case AArch64::UZP2v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v4i32\n");
            break;
        }
        case AArch64::UZP2v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v8i16\n");
            break;
        }
        case AArch64::UZP2v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " UZP2v8i8\n");
            break;
        }
        case AArch64::XTNv16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " XTNv16i8\n");
            break;
        }
        case AArch64::XTNv2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " XTNv2i32\n");
            break;
        }
        case AArch64::XTNv4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " XTNv4i16\n");
            break;
        }
        case AArch64::XTNv4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " XTNv4i32\n");
            break;
        }
        case AArch64::XTNv8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " XTNv8i16\n");
            break;
        }
        case AArch64::XTNv8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " XTNv8i8\n");
            break;
        }
        case AArch64::ZIP1v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v16i8\n");
            break;
        }
        case AArch64::ZIP1v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v2i32\n");
            break;
        }
        case AArch64::ZIP1v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v2i64\n");
            break;
        }
        case AArch64::ZIP1v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v4i16\n");
            break;
        }
        case AArch64::ZIP1v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v4i32\n");
            break;
        }
        case AArch64::ZIP1v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v8i16\n");
            break;
        }
        case AArch64::ZIP1v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP1v8i8\n");
            break;
        }
        case AArch64::ZIP2v16i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v16i8\n");
            break;
        }
        case AArch64::ZIP2v2i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v2i32\n");
            break;
        }
        case AArch64::ZIP2v2i64: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v2i64\n");
            break;
        }
        case AArch64::ZIP2v4i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v4i16\n");
            break;
        }
        case AArch64::ZIP2v4i32: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v4i32\n");
            break;
        }
        case AArch64::ZIP2v8i16: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v8i16\n");
            break;
        }
        case AArch64::ZIP2v8i8: {
            DEBUG(errs() << "0x" << utohexstr(Address) << " ZIP2v8i8\n");
            break;
        }
    }
}
