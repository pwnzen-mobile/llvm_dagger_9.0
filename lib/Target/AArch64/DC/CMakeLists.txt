include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMAARCH64DC
  AArch64DCInfo.cpp
  AArch64InstrSema.cpp
  AArch64RegisterSema.cpp
  AArch64InstrSemaDebug.cpp
  )

add_dependencies(LLVMAARCH64DC AArch64CommonTableGen)
