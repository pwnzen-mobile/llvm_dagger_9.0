#include "AArch64InstrSema.h"
#include "AArch64RegisterSema.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

DCInstrSema *createAArch64DCInstrSema(StringRef TT,
                                  DCRegisterSema &DRS,
                                  const MCRegisterInfo &MRI,
                                  const MCInstrInfo &MII) {
  (void)MRI;
  (void)MII;
  return new AArch64InstrSema(DRS);
}

DCRegisterSema *createAArch64DCRegisterSema(StringRef TT,
                                        const MCRegisterInfo &MRI,
                                        const MCInstrInfo &MII,
                                        const DataLayout &DL) {
  return new AArch64RegisterSema(MRI, MII, DL);
}

// Force static initialization.
extern "C" void LLVMInitializeAArch64TargetDC() {
  // These are only available for x86_64:
  // Register the DC instruction semantic info.
  TargetRegistry::RegisterDCInstrSema(TheAArch64leTarget,
                                      createAArch64DCInstrSema);
  TargetRegistry::RegisterDCInstrSema(TheAArch64beTarget,
                                      createAArch64DCInstrSema);

  // Register the DC register semantic info.
  TargetRegistry::RegisterDCRegisterSema(TheAArch64leTarget,
                                         createAArch64DCRegisterSema);
 TargetRegistry::RegisterDCRegisterSema(TheAArch64beTarget,
                                        createAArch64DCRegisterSema);

}
