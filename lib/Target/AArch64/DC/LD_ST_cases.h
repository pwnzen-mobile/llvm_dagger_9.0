
      case AArch64::ST1Onev16b:
      case AArch64::ST1Onev1d:
      case AArch64::ST1Onev2d:
      case AArch64::ST1Onev2s:
      case AArch64::ST1Onev4h:
      case AArch64::ST1Onev4s:
      case AArch64::ST1Onev8b:
      case AArch64::ST1Onev8h:
      case AArch64::ST1Twov16b:
      case AArch64::ST1Twov1d:
      case AArch64::ST1Twov2d:
      case AArch64::ST1Twov2s:
      case AArch64::ST1Twov4h:
      case AArch64::ST1Twov4s:
      case AArch64::ST1Twov8b:
      case AArch64::ST1Twov8h:
      case AArch64::ST1Threev16b:
      case AArch64::ST1Threev1d:
      case AArch64::ST1Threev2d:
      case AArch64::ST1Threev2s:
      case AArch64::ST1Threev4h:
      case AArch64::ST1Threev4s:
      case AArch64::ST1Threev8b:
      case AArch64::ST1Threev8h:
      case AArch64::ST1Fourv16b:
      case AArch64::ST1Fourv1d:
      case AArch64::ST1Fourv2d:
      case AArch64::ST1Fourv2s:
      case AArch64::ST1Fourv4h:
      case AArch64::ST1Fourv4s:
      case AArch64::ST1Fourv8b:
      case AArch64::ST1Fourv8h:
      case AArch64::ST2Twov16b:
      case AArch64::ST2Twov2d:
      case AArch64::ST2Twov2s:
      case AArch64::ST2Twov4h:
      case AArch64::ST2Twov4s:
      case AArch64::ST2Twov8b:
      case AArch64::ST2Twov8h:
      case AArch64::ST3Threev16b:
      case AArch64::ST3Threev2d:
      case AArch64::ST3Threev2s:
      case AArch64::ST3Threev4h:
      case AArch64::ST3Threev4s:
      case AArch64::ST3Threev8b:
      case AArch64::ST3Threev8h:
      case AArch64::ST4Fourv16b:
      case AArch64::ST4Fourv2d:
      case AArch64::ST4Fourv2s:
      case AArch64::ST4Fourv4h:
      case AArch64::ST4Fourv4s:
      case AArch64::ST4Fourv8b:
      case AArch64::ST4Fourv8h:
      {
        Value *val = getReg(CurrentInst->Inst.getOperand(0).getReg());
        Value *addr = getReg(CurrentInst->Inst.getOperand(1).getReg());
        addr = Builder->CreateIntToPtr(addr, val->getType()->getPointerTo());
        Builder->CreateStore(val, addr);
        return true;
      }

      case AArch64::ST1Onev16b_POST:
      case AArch64::ST1Onev1d_POST:
      case AArch64::ST1Onev2d_POST:
      case AArch64::ST1Onev2s_POST:
      case AArch64::ST1Onev4h_POST:
      case AArch64::ST1Onev4s_POST:
      case AArch64::ST1Onev8b_POST:
      case AArch64::ST1Onev8h_POST:
      case AArch64::ST1Twov16b_POST:
      case AArch64::ST1Twov1d_POST:
      case AArch64::ST1Twov2d_POST:
      case AArch64::ST1Twov2s_POST:
      case AArch64::ST1Twov4h_POST:
      case AArch64::ST1Twov4s_POST:
      case AArch64::ST1Twov8b_POST:
      case AArch64::ST1Twov8h_POST:
      case AArch64::ST1Threev16b_POST:
      case AArch64::ST1Threev1d_POST:
      case AArch64::ST1Threev2d_POST:
      case AArch64::ST1Threev2s_POST:
      case AArch64::ST1Threev4h_POST:
      case AArch64::ST1Threev4s_POST:
      case AArch64::ST1Threev8b_POST:
      case AArch64::ST1Threev8h_POST:
      case AArch64::ST1Fourv16b_POST:
      case AArch64::ST1Fourv1d_POST:
      case AArch64::ST1Fourv2d_POST:
      case AArch64::ST1Fourv2s_POST:
      case AArch64::ST1Fourv4h_POST:
      case AArch64::ST1Fourv4s_POST:
      case AArch64::ST1Fourv8b_POST:
      case AArch64::ST1Fourv8h_POST:
      case AArch64::ST2Twov16b_POST:
      case AArch64::ST2Twov2d_POST:
      case AArch64::ST2Twov2s_POST:
      case AArch64::ST2Twov4h_POST:
      case AArch64::ST2Twov4s_POST:
      case AArch64::ST2Twov8b_POST:
      case AArch64::ST2Twov8h_POST:
      case AArch64::ST3Threev16b_POST:
      case AArch64::ST3Threev2d_POST:
      case AArch64::ST3Threev2s_POST:
      case AArch64::ST3Threev4h_POST:
      case AArch64::ST3Threev4s_POST:
      case AArch64::ST3Threev8b_POST:
      case AArch64::ST3Threev8h_POST:
      case AArch64::ST4Fourv16b_POST:
      case AArch64::ST4Fourv2d_POST:
      case AArch64::ST4Fourv2s_POST:
      case AArch64::ST4Fourv4h_POST:
      case AArch64::ST4Fourv4s_POST:
      case AArch64::ST4Fourv8b_POST:
      case AArch64::ST4Fourv8h_POST:
      {
        unsigned int regNo = CurrentInst->Inst.getOperand(0).getReg();
        Value *val = getReg(CurrentInst->Inst.getOperand(1).getReg());
        Value *addr = getReg(regNo);
        Value *loadAddr = Builder->CreateIntToPtr(addr, val->getType()->getPointerTo());
        Builder->CreateStore(val, loadAddr);

        unsigned int incrementSize = 0;

        switch (Opcode) {
          default:
            llvm_unreachable("Instruction not handled");
            break;
          case AArch64::ST1Onev16b_POST:
          case AArch64::ST1Onev2d_POST:
          case AArch64::ST1Onev4s_POST:
          case AArch64::ST1Onev8h_POST:
            incrementSize = 16;
            break;
          case AArch64::ST1Onev1d_POST:
          case AArch64::ST1Onev2s_POST:
          case AArch64::ST1Onev4h_POST:
          case AArch64::ST1Onev8b_POST:
            incrementSize = 8;
            break;

          case AArch64::ST1Twov16b_POST:
          case AArch64::ST1Twov2d_POST:
          case AArch64::ST1Twov4s_POST:
          case AArch64::ST1Twov8h_POST:
            incrementSize = 32;
            break;
          case AArch64::ST1Twov1d_POST:
          case AArch64::ST1Twov2s_POST:
          case AArch64::ST1Twov4h_POST:
          case AArch64::ST1Twov8b_POST:
            incrementSize = 16;
            break;

          case AArch64::ST1Threev16b_POST:
          case AArch64::ST1Threev2d_POST:
          case AArch64::ST1Threev4s_POST:
          case AArch64::ST1Threev8h_POST:
            incrementSize = 48;
            break;
          case AArch64::ST1Threev1d_POST:
          case AArch64::ST1Threev2s_POST:
          case AArch64::ST1Threev4h_POST:
          case AArch64::ST1Threev8b_POST:
            incrementSize = 24;
            break;

          case AArch64::ST1Fourv16b_POST:
          case AArch64::ST1Fourv2d_POST:
          case AArch64::ST1Fourv4s_POST:
          case AArch64::ST1Fourv8h_POST:
            incrementSize = 64;
            break;
          case AArch64::ST1Fourv1d_POST:
          case AArch64::ST1Fourv2s_POST:
          case AArch64::ST1Fourv4h_POST:
          case AArch64::ST1Fourv8b_POST:
            incrementSize = 32;
            break;

          case AArch64::ST2Twov16b_POST:
          case AArch64::ST2Twov2d_POST:
          case AArch64::ST2Twov4s_POST:
          case AArch64::ST2Twov8h_POST:
            incrementSize = 32;
            break;

          case AArch64::ST2Twov2s_POST:
          case AArch64::ST2Twov4h_POST:
          case AArch64::ST2Twov8b_POST:
            incrementSize = 16;
            break;

          case AArch64::ST3Threev16b_POST:
          case AArch64::ST3Threev2d_POST:
          case AArch64::ST3Threev4s_POST:
          case AArch64::ST3Threev8h_POST:
            incrementSize = 48;
            break;

          case AArch64::ST3Threev2s_POST:
          case AArch64::ST3Threev4h_POST:
          case AArch64::ST3Threev8b_POST:
            incrementSize = 24;
            break;

          case AArch64::ST4Fourv16b_POST:
          case AArch64::ST4Fourv2d_POST:
          case AArch64::ST4Fourv4s_POST:
          case AArch64::ST4Fourv8h_POST:
            incrementSize = 64;
            break;

          case AArch64::ST4Fourv2s_POST:
          case AArch64::ST4Fourv4h_POST:
          case AArch64::ST4Fourv8b_POST:
            incrementSize = 32;
            break;
        }

        addr = Builder->CreateAdd(addr, Builder->getInt(APInt(addr->getType()->getScalarSizeInBits(), incrementSize)));
        setReg(regNo, addr);
        return true;
      }

      case AArch64::LD1Onev16b:
      case AArch64::LD1Onev1d:
      case AArch64::LD1Onev2d:
      case AArch64::LD1Onev2s:
      case AArch64::LD1Onev4h:
      case AArch64::LD1Onev4s:
      case AArch64::LD1Onev8b:
      case AArch64::LD1Onev8h:
      case AArch64::LD1Twov16b:
      case AArch64::LD1Twov1d:
      case AArch64::LD1Twov2d:
      case AArch64::LD1Twov2s:
      case AArch64::LD1Twov4h:
      case AArch64::LD1Twov4s:
      case AArch64::LD1Twov8b:
      case AArch64::LD1Twov8h:
      case AArch64::LD1Threev16b:
      case AArch64::LD1Threev1d:
      case AArch64::LD1Threev2d:
      case AArch64::LD1Threev2s:
      case AArch64::LD1Threev4h:
      case AArch64::LD1Threev4s:
      case AArch64::LD1Threev8b:
      case AArch64::LD1Threev8h:
      case AArch64::LD1Fourv16b:
      case AArch64::LD1Fourv1d:
      case AArch64::LD1Fourv2d:
      case AArch64::LD1Fourv2s:
      case AArch64::LD1Fourv4h:
      case AArch64::LD1Fourv4s:
      case AArch64::LD1Fourv8b:
      case AArch64::LD1Fourv8h:

      case AArch64::LD2Twov16b:
      case AArch64::LD2Twov2d:
      case AArch64::LD2Twov2s:
      case AArch64::LD2Twov4h:
      case AArch64::LD2Twov4s:
      case AArch64::LD2Twov8b:
      case AArch64::LD2Twov8h:
      case AArch64::LD3Threev16b:
      case AArch64::LD3Threev2d:
      case AArch64::LD3Threev2s:
      case AArch64::LD3Threev4h:
      case AArch64::LD3Threev4s:
      case AArch64::LD3Threev8b:
      case AArch64::LD3Threev8h:
      case AArch64::LD4Fourv16b:
      case AArch64::LD4Fourv2d:
      case AArch64::LD4Fourv2s:
      case AArch64::LD4Fourv4h:
      case AArch64::LD4Fourv4s:
      case AArch64::LD4Fourv8b:
      case AArch64::LD4Fourv8h:
      {
        Type *loadType = nullptr;

        unsigned int regNo = CurrentInst->Inst.getOperand(0).getReg();
        if (regNo >= AArch64::B0 && regNo <= AArch64::B31) {
          loadType = IntegerType::get(getGlobalContext(), 8);
        } else if (regNo >= AArch64::H0 && regNo <= AArch64::H31) {
          loadType = IntegerType::get(getGlobalContext(), 16);
        } else if (regNo >= AArch64::S0 && regNo <= AArch64::S31) {
          loadType = IntegerType::get(getGlobalContext(), 32);
        } else if (regNo >= AArch64::D0 && regNo <= AArch64::D31) {
          loadType = IntegerType::get(getGlobalContext(), 64);
        } else if (regNo >= AArch64::Q0 && regNo <= AArch64::Q31) {
          loadType = IntegerType::get(getGlobalContext(), 128);
        } else if (regNo >= AArch64::Q0_Q1 && regNo <= AArch64::Q31_Q0) {
          loadType = IntegerType::get(getGlobalContext(), 256);
        } else if (regNo >= AArch64::Q0_Q1_Q2 && regNo <= AArch64::Q31_Q0_Q1) {
          loadType = IntegerType::get(getGlobalContext(), 384);
        } else if (regNo >= AArch64::Q0_Q1_Q2_Q3 && regNo <= AArch64::Q31_Q0_Q1_Q2) {
          loadType = IntegerType::get(getGlobalContext(), 512);
        } else if (regNo >= AArch64::D0_D1 && regNo <= AArch64::D31_D0) {
          loadType = IntegerType::get(getGlobalContext(), 128);
        } else if (regNo >= AArch64::D0_D1_D2 && regNo <= AArch64::D31_D0_D1) {
          loadType = IntegerType::get(getGlobalContext(), 192);
        } else if (regNo >= AArch64::D0_D1_D2_D3 && regNo <= AArch64::D31_D0_D1_D2) {
          loadType = IntegerType::get(getGlobalContext(), 256);
        } else {
          llvm_unreachable("Registers not handled");
        }

        loadType = loadType->getPointerTo();

        Value *addr = getReg(CurrentInst->Inst.getOperand(1).getReg());
        addr = Builder->CreateIntToPtr(addr, loadType);

        Value *val = Builder->CreateLoad(addr);
        setReg(regNo, val);
        return true;
      }

      case AArch64::LD1Onev16b_POST:
      case AArch64::LD1Onev1d_POST:
      case AArch64::LD1Onev2d_POST:
      case AArch64::LD1Onev2s_POST:
      case AArch64::LD1Onev4h_POST:
      case AArch64::LD1Onev4s_POST:
      case AArch64::LD1Onev8b_POST:
      case AArch64::LD1Onev8h_POST:
      case AArch64::LD1Twov16b_POST:
      case AArch64::LD1Twov1d_POST:
      case AArch64::LD1Twov2d_POST:
      case AArch64::LD1Twov2s_POST:
      case AArch64::LD1Twov4h_POST:
      case AArch64::LD1Twov4s_POST:
      case AArch64::LD1Twov8b_POST:
      case AArch64::LD1Twov8h_POST:
      case AArch64::LD1Threev16b_POST:
      case AArch64::LD1Threev1d_POST:
      case AArch64::LD1Threev2d_POST:
      case AArch64::LD1Threev2s_POST:
      case AArch64::LD1Threev4h_POST:
      case AArch64::LD1Threev4s_POST:
      case AArch64::LD1Threev8b_POST:
      case AArch64::LD1Threev8h_POST:
      case AArch64::LD1Fourv16b_POST:
      case AArch64::LD1Fourv1d_POST:
      case AArch64::LD1Fourv2d_POST:
      case AArch64::LD1Fourv2s_POST:
      case AArch64::LD1Fourv4h_POST:
      case AArch64::LD1Fourv4s_POST:
      case AArch64::LD1Fourv8b_POST:
      case AArch64::LD1Fourv8h_POST:
      case AArch64::LD2Twov16b_POST:
      case AArch64::LD2Twov2d_POST:
      case AArch64::LD2Twov2s_POST:
      case AArch64::LD2Twov4h_POST:
      case AArch64::LD2Twov4s_POST:
      case AArch64::LD2Twov8b_POST:
      case AArch64::LD2Twov8h_POST:
      case AArch64::LD3Threev16b_POST:
      case AArch64::LD3Threev2d_POST:
      case AArch64::LD3Threev2s_POST:
      case AArch64::LD3Threev4h_POST:
      case AArch64::LD3Threev4s_POST:
      case AArch64::LD3Threev8b_POST:
      case AArch64::LD3Threev8h_POST:
      case AArch64::LD4Fourv16b_POST:
      case AArch64::LD4Fourv2d_POST:
      case AArch64::LD4Fourv2s_POST:
      case AArch64::LD4Fourv4h_POST:
      case AArch64::LD4Fourv4s_POST:
      case AArch64::LD4Fourv8b_POST:
      case AArch64::LD4Fourv8h_POST:
      {
        Type *loadType = nullptr;

        unsigned int regNo = CurrentInst->Inst.getOperand(1).getReg();
        if (regNo >= AArch64::B0 && regNo <= AArch64::B31) {
          loadType = IntegerType::get(getGlobalContext(), 8);
        } else if (regNo >= AArch64::S0 && regNo <= AArch64::S31) {
          loadType = IntegerType::get(getGlobalContext(), 16);
        } else if (regNo >= AArch64::S0 && regNo <= AArch64::S31) {
          loadType = IntegerType::get(getGlobalContext(), 32);
        } else if (regNo >= AArch64::D0 && regNo <= AArch64::D31) {
          loadType = IntegerType::get(getGlobalContext(), 64);
        } else if (regNo >= AArch64::Q0 && regNo <= AArch64::Q31) {
          loadType = IntegerType::get(getGlobalContext(), 128);
        } else if (regNo >= AArch64::Q0_Q1 && regNo <= AArch64::Q31_Q0) {
          loadType = IntegerType::get(getGlobalContext(), 256);
        } else if (regNo >= AArch64::Q0_Q1_Q2 && regNo <= AArch64::Q31_Q0_Q1) {
          loadType = IntegerType::get(getGlobalContext(), 384);
        } else if (regNo >= AArch64::Q0_Q1_Q2_Q3 && regNo <= AArch64::Q31_Q0_Q1_Q2) {
          loadType = IntegerType::get(getGlobalContext(), 512);
        } else if (regNo >= AArch64::D0_D1 && regNo <= AArch64::D31_D0) {
          loadType = IntegerType::get(getGlobalContext(), 128);
        } else if (regNo >= AArch64::D0_D1_D2 && regNo <= AArch64::D31_D0_D1) {
          loadType = IntegerType::get(getGlobalContext(), 192);
        } else if (regNo >= AArch64::D0_D1_D2_D3 && regNo <= AArch64::D31_D0_D1_D2) {
          loadType = IntegerType::get(getGlobalContext(), 256);
        } else {
          llvm_unreachable("Registers not handled");
        }

        loadType = loadType->getPointerTo();

        unsigned int loadRegNo = CurrentInst->Inst.getOperand(0).getReg();
        Value *addr = getReg(loadRegNo);
        Value *loadAddr = Builder->CreateIntToPtr(addr, loadType);
        Value *val = Builder->CreateLoad(loadAddr);
        //errs() << "[+]addr: " << *addr << "\tloadAddr: " << *loadAddr << "\tval: " << *val << "\n";
        
        /*
        unsigned int loadRegNo1 = CurrentInst->Inst.getOperand(2).getReg();
        Value *addr1 = getReg(loadRegNo1);
        Value *loadAddr1 = Builder->CreateIntToPtr(addr1, loadType);
        Value *val1 = Builder->CreateLoad(loadAddr1);
        errs() << "[+]addr1: " << *addr1 << "\tloadAddr1: " << *loadAddr1 << "\tval1: " << *val1 << "\n";
        */
        setReg(regNo, val);

        unsigned int incrementSize = 0;
        switch(Opcode) {
          default:
            llvm_unreachable("unhandled instruction");
            break;


          case AArch64::LD1Fourv16b_POST:
          case AArch64::LD1Fourv2d_POST:
          case AArch64::LD1Fourv4s_POST:
          case AArch64::LD1Fourv8h_POST:
          case AArch64::LD4Fourv16b_POST:
          case AArch64::LD4Fourv2d_POST:
          case AArch64::LD4Fourv4s_POST:
          case AArch64::LD4Fourv8h_POST:
            incrementSize = 64;
            break;

          case AArch64::LD1Threev16b_POST:
          case AArch64::LD1Threev2d_POST:
          case AArch64::LD1Threev4s_POST:
          case AArch64::LD1Threev8h_POST:
          case AArch64::LD3Threev16b_POST:
          case AArch64::LD3Threev2d_POST:
          case AArch64::LD3Threev4s_POST:
          case AArch64::LD3Threev8h_POST:
            incrementSize = 48;
            break;

          case AArch64::LD1Fourv1d_POST:
          case AArch64::LD1Fourv2s_POST:
          case AArch64::LD1Fourv4h_POST:
          case AArch64::LD1Fourv8b_POST:
          case AArch64::LD1Twov16b_POST:
          case AArch64::LD1Twov2d_POST:
          case AArch64::LD1Twov4s_POST:
          case AArch64::LD1Twov8h_POST:
          case AArch64::LD2Twov16b_POST:
          case AArch64::LD2Twov2d_POST:
          case AArch64::LD2Twov4s_POST:
          case AArch64::LD2Twov8h_POST:
          case AArch64::LD4Fourv2s_POST:
          case AArch64::LD4Fourv4h_POST:
          case AArch64::LD4Fourv8b_POST:
            incrementSize = 32;
            break;

          case AArch64::LD1Threev1d_POST:
          case AArch64::LD1Threev2s_POST:
          case AArch64::LD1Threev4h_POST:
          case AArch64::LD1Threev8b_POST:
          case AArch64::LD3Threev2s_POST:
          case AArch64::LD3Threev4h_POST:
          case AArch64::LD3Threev8b_POST:
            incrementSize = 24;
            break;

          case AArch64::LD1Twov1d_POST:
          case AArch64::LD1Twov2s_POST:
          case AArch64::LD1Twov4h_POST:
          case AArch64::LD1Twov8b_POST:
          case AArch64::LD1Onev16b_POST:
          case AArch64::LD1Onev2d_POST:
          case AArch64::LD1Onev8h_POST:
          case AArch64::LD2Twov2s_POST:
          case AArch64::LD2Twov4h_POST:
          case AArch64::LD2Twov8b_POST:
            incrementSize = 16;
            break;

          case AArch64::LD1Onev1d_POST:
          case AArch64::LD1Onev2s_POST:
          case AArch64::LD1Onev4h_POST:
          case AArch64::LD1Onev4s_POST:
          case AArch64::LD1Onev8b_POST:
            incrementSize = 8;
            break;
        }

        addr = Builder->CreateAdd(addr, Builder->getInt(APInt(addr->getType()->getScalarSizeInBits(), incrementSize)));
        setReg(loadRegNo, addr);

        return true;
      }

      case AArch64::ST1i16:
      case AArch64::ST1i32:
      case AArch64::ST1i64:
      case AArch64::ST1i8:
      case AArch64::ST2i16:
      case AArch64::ST2i32:
      case AArch64::ST2i64:
      case AArch64::ST2i8:
      case AArch64::ST3i16:
      case AArch64::ST3i32:
      case AArch64::ST3i64:
      case AArch64::ST3i8:
      case AArch64::ST4i16:
      case AArch64::ST4i32:
      case AArch64::ST4i64:
      case AArch64::ST4i8:
      {
        unsigned int dstRegNo = CurrentInst->Inst.getOperand(2).getReg();
        unsigned int srcRegNo = CurrentInst->Inst.getOperand(0).getReg();
        unsigned int index = CurrentInst->Inst.getOperand(1).getImm();

        unsigned int numVectors = 0;
        unsigned int numElements = 0;
        Type *elementType = nullptr;

        switch (Opcode) {
          default:
            llvm_unreachable("not handled");
            break;
          case AArch64::ST1i16:
            numVectors = 1;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::ST1i32:
            numVectors = 1;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::ST1i64:
            numVectors = 1;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::ST1i8:
            numVectors = 1;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::ST2i16:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::ST2i32:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::ST2i64:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::ST2i8:
            numVectors = 2;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::ST3i16:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::ST3i32:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::ST3i64:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(),64);
            break;
          case AArch64::ST3i8:
            numVectors = 3;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::ST4i16:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::ST4i32:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::ST4i64:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::ST4i8:
            numVectors = 4;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
        }

        Type *srcType = VectorType::get(elementType, numVectors * numElements);

        Value *src = getReg(srcRegNo);
        src = Builder->CreateBitCast(src, srcType);

        Value *dst = Builder->getInt(APInt(elementType->getScalarSizeInBits() * numVectors, 0));
        dst = Builder->CreateBitCast(dst, VectorType::get(elementType, numVectors));

        for (unsigned i = 0; i < numVectors; ++i) {
          Value *elem = Builder->CreateExtractElement(src, (i * numElements) + index);
          dst = Builder->CreateInsertElement(dst, elem, i);
        }

        dst = Builder->CreateBitCast(dst, IntegerType::get(getGlobalContext(), elementType->getScalarSizeInBits() * numVectors));

        Value *store = getReg(dstRegNo);
        Value *storeAddress = Builder->CreateIntToPtr(store, dst->getType()->getPointerTo());


        Builder->CreateStore(dst, storeAddress);

        return true;
      }

      case AArch64::ST1i16_POST:
      case AArch64::ST1i32_POST:
      case AArch64::ST1i64_POST:
      case AArch64::ST1i8_POST:
      case AArch64::ST2i16_POST:
      case AArch64::ST2i32_POST:
      case AArch64::ST2i64_POST:
      case AArch64::ST2i8_POST:
      case AArch64::ST3i16_POST:
      case AArch64::ST3i32_POST:
      case AArch64::ST3i64_POST:
      case AArch64::ST3i8_POST:
      case AArch64::ST4i16_POST:
      case AArch64::ST4i32_POST:
      case AArch64::ST4i64_POST:
      case AArch64::ST4i8_POST:
      {
        //WTF, src dst reg?
        unsigned int dstRegNo = CurrentInst->Inst.getOperand(0).getReg();
        unsigned int srcRegNo = CurrentInst->Inst.getOperand(1).getReg();
        unsigned int index = CurrentInst->Inst.getOperand(2).getImm();


        //unsigned int dstRegNo = CurrentInst->Inst.getOperand(2).getReg();
        //unsigned int srcRegNo = CurrentInst->Inst.getOperand(0).getReg();
        //unsigned int index = CurrentInst->Inst.getOperand(1).getImm();

        unsigned int numVectors = 0;
        unsigned int numElements = 0;
        Type *elementType = nullptr;
        unsigned int incrementSize = 0;

        switch (Opcode) {
          default:
            llvm_unreachable("not handled");
            break;
          case AArch64::ST1i16_POST:
            numVectors = 1;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 2;
            break;
          case AArch64::ST1i32_POST:
            numVectors = 1;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 4;
            break;
          case AArch64::ST1i64_POST:
            numVectors = 1;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 8;
            break;
          case AArch64::ST1i8_POST:
            numVectors = 1;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 1;
            break;
          case AArch64::ST2i16_POST:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 4;
            break;
          case AArch64::ST2i32_POST:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 8;
            break;
          case AArch64::ST2i64_POST:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 16;
            break;
          case AArch64::ST2i8_POST:
            numVectors = 2;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 2;
            break;
          case AArch64::ST3i16_POST:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 6;
            break;
          case AArch64::ST3i32_POST:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 12;
            break;
          case AArch64::ST3i64_POST:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(),64);
            incrementSize = 24;
            break;
          case AArch64::ST3i8_POST:
            numVectors = 3;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 3;
            break;
          case AArch64::ST4i16_POST:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 8;
            break;
          case AArch64::ST4i32_POST:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 16;
            break;
          case AArch64::ST4i64_POST:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 32;
            break;
          case AArch64::ST4i8_POST:
            numVectors = 4;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 4;
            break;
        }

        Type *srcType = VectorType::get(elementType, numVectors * numElements);

        Value *src = getReg(srcRegNo);
        src = Builder->CreateBitCast(src, srcType);

        Value *dst = Builder->getInt(APInt(elementType->getScalarSizeInBits() * numVectors, 0));
        dst = Builder->CreateBitCast(dst, VectorType::get(elementType, numVectors));

        for (unsigned i = 0; i < numVectors; ++i) {
          Value *elem = Builder->CreateExtractElement(src, (i * numElements) + index);
          dst = Builder->CreateInsertElement(dst, elem, i);
        }

        dst = Builder->CreateBitCast(dst, IntegerType::get(getGlobalContext(), elementType->getScalarSizeInBits() * numVectors));

        Value *store = getReg(dstRegNo);
        Value *storeAddress = Builder->CreateIntToPtr(store, dst->getType()->getPointerTo());


        Builder->CreateStore(dst, storeAddress);

        store = Builder->CreateAdd(store, Builder->getInt(APInt(store->getType()->getScalarSizeInBits(), incrementSize)));
        setReg(dstRegNo, store);

        return true;
      }
      
      case AArch64::LD2i16:
      case AArch64::LD2i32:
      case AArch64::LD2i64:
      case AArch64::LD2i8:
      case AArch64::LD3i16:
      case AArch64::LD3i32:
      case AArch64::LD3i64:
      case AArch64::LD3i8:
      case AArch64::LD4i16:
      case AArch64::LD4i32:
      case AArch64::LD4i64:
      case AArch64::LD4i8:
      {
        unsigned int dstRegNo = CurrentInst->Inst.getOperand(0).getReg();
        unsigned int srcRegNo = CurrentInst->Inst.getOperand(3).getReg();
        unsigned int index = CurrentInst->Inst.getOperand(2).getImm();

        unsigned int numVectors = 0;
        unsigned int numElements = 0;
        Type *elementType = nullptr;

        switch(Opcode) {
          case AArch64::LD2i16:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD2i32:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD2i64:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD2i8:
            numVectors = 2;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;

          case AArch64::LD3i16:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD3i32:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD3i64:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD3i8:
            numVectors = 3;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;

          case AArch64::LD4i16:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD4i32:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD4i64:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD4i8:
            numVectors = 4;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
        }

        Value *dst = getReg(dstRegNo);
        Type *dstType = dst->getType();
        dst = Builder->CreateBitCast(dst, VectorType::get(elementType, numVectors * numElements));

        Value *loadReg = getReg(srcRegNo);
        Value *loadAddr = Builder->CreateIntToPtr(loadReg, VectorType::get(elementType, numVectors)->getPointerTo());

        Value *load = Builder->CreateLoad(loadAddr);

        for (unsigned i = 0; i < numVectors; ++i) {
          Value *elem = Builder->CreateExtractElement(load, i);
          dst = Builder->CreateInsertElement(dst, elem, (i * numElements) + index);
        }
        dst = Builder->CreateBitCast(dst, dstType);
        setReg(dstRegNo, dst);

        return true;
      }

      case AArch64::LD1Rv16b:
      case AArch64::LD1Rv1d:
      case AArch64::LD1Rv2d:
      case AArch64::LD1Rv2s:
      case AArch64::LD1Rv4h:
      case AArch64::LD1Rv4s:
      case AArch64::LD1Rv8b:
      case AArch64::LD1Rv8h:
      case AArch64::LD2Rv16b:
      case AArch64::LD2Rv1d:
      case AArch64::LD2Rv2d:
      case AArch64::LD2Rv2s:
      case AArch64::LD2Rv4h:
      case AArch64::LD2Rv4s:
      case AArch64::LD2Rv8b:
      case AArch64::LD2Rv8h:
      case AArch64::LD3Rv16b:
      case AArch64::LD3Rv1d:
      case AArch64::LD3Rv2d:
      case AArch64::LD3Rv2s:
      case AArch64::LD3Rv4h:
      case AArch64::LD3Rv4s:
      case AArch64::LD3Rv8b:
      case AArch64::LD3Rv8h:
      case AArch64::LD4Rv16b:
      case AArch64::LD4Rv1d:
      case AArch64::LD4Rv2d:
      case AArch64::LD4Rv2s:
      case AArch64::LD4Rv4h:
      case AArch64::LD4Rv4s:
      case AArch64::LD4Rv8b:
      case AArch64::LD4Rv8h:
      {
        unsigned int srcReg = CurrentInst->Inst.getOperand(1).getReg();
        unsigned int dstReg = CurrentInst->Inst.getOperand(0).getReg();

        unsigned int numVectors = 0;
        unsigned int numElements = 0;
        Type *elementType = nullptr;

        switch (Opcode) {
          default:
            llvm_unreachable("not handled");
            break;
          case AArch64::LD1Rv16b:
            numVectors = 1;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD1Rv1d:
            numVectors = 1;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD1Rv2d:
            numVectors = 1;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD1Rv2s:
            numVectors = 1;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD1Rv4h:
            numVectors = 1;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD1Rv4s:
            numVectors = 1;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD1Rv8b:
            numVectors = 1;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD1Rv8h:
            numVectors = 1;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD2Rv16b:
            numVectors = 2;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD2Rv1d:
            numVectors = 2;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD2Rv2d:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD2Rv2s:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD2Rv4h:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD2Rv4s:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD2Rv8b:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD2Rv8h:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD3Rv16b:
            numVectors = 3;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD3Rv1d:
            numVectors = 3;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD3Rv2d:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD3Rv2s:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD3Rv4h:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD3Rv4s:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD3Rv8b:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD3Rv8h:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD4Rv16b:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD4Rv1d:
            numVectors = 4;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD4Rv2d:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            break;
          case AArch64::LD4Rv2s:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD4Rv4h:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
          case AArch64::LD4Rv4s:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            break;
          case AArch64::LD4Rv8b:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            break;
          case AArch64::LD4Rv8h:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            break;
        }


        Value *src = getReg(srcReg);
        Value *srcAddress = Builder->CreateIntToPtr(src, VectorType::get(elementType, numVectors)->getPointerTo());
        Value *val = Builder->CreateLoad(srcAddress);

        Value *dstVec = Builder->getInt(APInt(numVectors * numElements * elementType->getScalarSizeInBits(), 0));
        dstVec = Builder->CreateBitCast(dstVec, VectorType::get(elementType, numVectors * numElements));

        for (unsigned i = 0; i < numVectors; ++i) {
          Value *elem = Builder->CreateExtractElement(val, i);

          for (unsigned j = 0; j < numElements; ++j) {
            dstVec = Builder->CreateInsertElement(dstVec, elem, (i * numElements) + j);
          }
        }

        dstVec = Builder->CreateBitCast(dstVec, IntegerType::get(getGlobalContext(), numVectors * numElements * elementType->getScalarSizeInBits()));
        setReg(dstReg, dstVec);
        return true;
      }

      case AArch64::LD1Rv16b_POST:
      case AArch64::LD1Rv1d_POST:
      case AArch64::LD1Rv2d_POST:
      case AArch64::LD1Rv2s_POST:
      case AArch64::LD1Rv4h_POST:
      case AArch64::LD1Rv4s_POST:
      case AArch64::LD1Rv8b_POST:
      case AArch64::LD1Rv8h_POST:
      case AArch64::LD2Rv16b_POST:
      case AArch64::LD2Rv1d_POST:
      case AArch64::LD2Rv2d_POST:
      case AArch64::LD2Rv2s_POST:
      case AArch64::LD2Rv4h_POST:
      case AArch64::LD2Rv4s_POST:
      case AArch64::LD2Rv8b_POST:
      case AArch64::LD2Rv8h_POST:
      case AArch64::LD3Rv16b_POST:
      case AArch64::LD3Rv1d_POST:
      case AArch64::LD3Rv2d_POST:
      case AArch64::LD3Rv2s_POST:
      case AArch64::LD3Rv4h_POST:
      case AArch64::LD3Rv4s_POST:
      case AArch64::LD3Rv8b_POST:
      case AArch64::LD3Rv8h_POST:
      case AArch64::LD4Rv16b_POST:
      case AArch64::LD4Rv1d_POST:
      case AArch64::LD4Rv2d_POST:
      case AArch64::LD4Rv2s_POST:
      case AArch64::LD4Rv4h_POST:
      case AArch64::LD4Rv4s_POST:
      case AArch64::LD4Rv8b_POST:
      case AArch64::LD4Rv8h_POST:
      {
        unsigned int srcReg = CurrentInst->Inst.getOperand(0).getReg();
        unsigned int dstReg = CurrentInst->Inst.getOperand(1).getReg();

        unsigned int numVectors = 0;
        unsigned int numElements = 0;
        Type *elementType = nullptr;
        unsigned int incrementSize = 0;

        switch (Opcode) {
          default:
            llvm_unreachable("not handled");
            break;
          case AArch64::LD1Rv16b_POST:
            numVectors = 1;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 1;
            break;
          case AArch64::LD1Rv1d_POST:
            numVectors = 1;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 8;
            break;
          case AArch64::LD1Rv2d_POST:
            numVectors = 1;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 8;
            break;
          case AArch64::LD1Rv2s_POST:
            numVectors = 1;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 4;
            break;
          case AArch64::LD1Rv4h_POST:
            numVectors = 1;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 2;
            break;
          case AArch64::LD1Rv4s_POST:
            numVectors = 1;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 4;
            break;
          case AArch64::LD1Rv8b_POST:
            numVectors = 1;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 1;
            break;
          case AArch64::LD1Rv8h_POST:
            numVectors = 1;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 2;
            break;
          case AArch64::LD2Rv16b_POST:
            numVectors = 2;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 2;
            break;
          case AArch64::LD2Rv1d_POST:
            numVectors = 2;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 16;
            break;
          case AArch64::LD2Rv2d_POST:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 16;
            break;
          case AArch64::LD2Rv2s_POST:
            numVectors = 2;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 8;
            break;
          case AArch64::LD2Rv4h_POST:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 4;
            break;
          case AArch64::LD2Rv4s_POST:
            numVectors = 2;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 8;
            break;
          case AArch64::LD2Rv8b_POST:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 2;
            break;
          case AArch64::LD2Rv8h_POST:
            numVectors = 2;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 4;
            break;
          case AArch64::LD3Rv16b_POST:
            numVectors = 3;
            numElements = 16;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 3;
            break;
          case AArch64::LD3Rv1d_POST:
            numVectors = 3;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 24;
            break;
          case AArch64::LD3Rv2d_POST:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 24;
            break;
          case AArch64::LD3Rv2s_POST:
            numVectors = 3;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 12;
            break;
          case AArch64::LD3Rv4h_POST:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 6;
            break;
          case AArch64::LD3Rv4s_POST:
            numVectors = 3;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 12;
            break;
          case AArch64::LD3Rv8b_POST:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 3;
            break;
          case AArch64::LD3Rv8h_POST:
            numVectors = 3;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 6;
            break;
          case AArch64::LD4Rv16b_POST:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 4;
            break;
          case AArch64::LD4Rv1d_POST:
            numVectors = 4;
            numElements = 1;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 32;
            break;
          case AArch64::LD4Rv2d_POST:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 64);
            incrementSize = 32;
            break;
          case AArch64::LD4Rv2s_POST:
            numVectors = 4;
            numElements = 2;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 16;
            break;
          case AArch64::LD4Rv4h_POST:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 8;
            break;
          case AArch64::LD4Rv4s_POST:
            numVectors = 4;
            numElements = 4;
            elementType = IntegerType::get(getGlobalContext(), 32);
            incrementSize = 16;
            break;
          case AArch64::LD4Rv8b_POST:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 8);
            incrementSize = 4;
            break;
          case AArch64::LD4Rv8h_POST:
            numVectors = 4;
            numElements = 8;
            elementType = IntegerType::get(getGlobalContext(), 16);
            incrementSize = 8;
            break;
        }


        Value *src = getReg(srcReg);
        Value *srcAddress = Builder->CreateIntToPtr(src, VectorType::get(elementType, numVectors)->getPointerTo());
        Value *val = Builder->CreateLoad(srcAddress);

        Value *dstVec = Builder->getInt(APInt(numVectors * numElements * elementType->getScalarSizeInBits(), 0));
        dstVec = Builder->CreateBitCast(dstVec, VectorType::get(elementType, numVectors * numElements));

        for (unsigned i = 0; i < numVectors; ++i) {
          Value *elem = Builder->CreateExtractElement(val, i);

          for (unsigned j = 0; j < numElements; ++j) {
            dstVec = Builder->CreateInsertElement(dstVec, elem, (i * numElements) + j);
          }
        }

        dstVec = Builder->CreateBitCast(dstVec, IntegerType::get(getGlobalContext(), numVectors * numElements * elementType->getScalarSizeInBits()));
        setReg(dstReg, dstVec);

        src = Builder->CreateAdd(src, Builder->getInt(APInt(src->getType()->getScalarSizeInBits(), incrementSize)));
        setReg(srcReg, src);

        return true;
      }
