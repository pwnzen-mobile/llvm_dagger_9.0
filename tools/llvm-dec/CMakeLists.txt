set(LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  BitReader
  BitWriter
  Core
  IRReader
  Linker
  Object
  Support
  TransformUtils
  IPO
  MCAnalysis
  MCDisassembler
  DC
  Core
  BinaryFormat
  InstCombine
  BitstreamReader
  MCParser
  MC
  )

add_llvm_tool(llvm-dec
  llvm-dec.cpp
  FunctionNamePass.cpp
  TailCallPass.cpp
  )

