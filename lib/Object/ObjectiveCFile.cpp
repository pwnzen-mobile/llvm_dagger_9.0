#include <llvm/ADT/StringExtras.h>
#include "llvm/Object/ObjectiveCFile.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/LEB128.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "object-c"
using namespace llvm;
using namespace object;


void ObjectiveCFile::resolveMethods() {
    for(section_iterator S_it = MachO->section_begin(); S_it != MachO->section_end(); ++S_it){
        StringRef SectionName;
        S_it->getName(SectionName);
        if (SectionName == "__objc_classlist") {
            ObjcClasslistAddress = S_it->getAddress();
            StringRef ObjcClasslistContent;
            S_it->getContents(ObjcClasslistContent);
            ObjcClasslistData = ArrayRef<uint8_t>((uint8_t*)ObjcClasslistContent.data(), ObjcClasslistContent.size());
        } else if (SectionName == "__objc_data") {
            ObjcDataAddress = S_it->getAddress();
            StringRef ObjcDataContent;
            S_it->getContents(ObjcDataContent);
            ObjcDataData = ArrayRef<uint8_t>((uint8_t*)ObjcDataContent.data(), ObjcDataContent.size());
        } else if (SectionName == "__data") {
            DataAddress = S_it->getAddress();
            StringRef DataContent;
            S_it->getContents(DataContent);
            DataData = ArrayRef<uint8_t>((uint8_t*)DataContent.data(), DataContent.size());
        } else if (SectionName == "__objc_const") {
            ObjcConstAddress = S_it->getAddress();
            StringRef ObjcConstContent;
            S_it->getContents(ObjcConstContent);
            ObjcConstData = ArrayRef<uint8_t>((uint8_t*)ObjcConstContent.data(), ObjcConstContent.size());
        } else if (SectionName == "__objc_classname") {
            ObjcClassnamesAddress = S_it->getAddress();
            StringRef ObjcClassnamesContent;
            S_it->getContents(ObjcClassnamesContent);
            ObjcClassnamesData = ArrayRef<uint8_t>((uint8_t*)ObjcClassnamesContent.data(), ObjcClassnamesContent.size());
        } else if (SectionName == "__cstring") {
            cStringsAddress = S_it->getAddress();
            StringRef cStringsContent;
            S_it->getContents(cStringsContent);
            //DEBUG(errs() << "[+]cStringsContent.data address: " << cStringsContent.data() 
              //  << "\tcStringsContent.size: " << cStringsContent.size() << "\n");
            cStringsData = ArrayRef<uint8_t>((uint8_t*)cStringsContent.data(), cStringsContent.size());
        } else if (SectionName == "__objc_methname") {
            ObjcMethodnamesAddress = S_it->getAddress();
            StringRef ObjcMethodnamesContent;
            S_it->getContents(ObjcMethodnamesContent);
            ObjcMethodnamesData = ArrayRef<uint8_t>((uint8_t*)ObjcMethodnamesContent.data(), ObjcMethodnamesContent.size());
        } else if (SectionName == "__objc_catlist") {
            ObjcCatlistAddress = S_it->getAddress();
            StringRef ObjcCatlistContent;
            S_it->getContents(ObjcCatlistContent);
            ObjcCatlistData = ArrayRef<uint8_t>((uint8_t*)ObjcCatlistContent.data(), ObjcCatlistContent.size());
        }
    }

    if (!(ObjcClasslistAddress && ObjcClasslistData.size())) {
        return;
    }

    assert(ObjcClasslistAddress && ObjcClasslistData.size());
    assert(ObjcDataAddress && ObjcDataData.size());
    assert(ObjcConstAddress && ObjcConstData.size());
    assert(ObjcClassnamesAddress && ObjcClassnamesData.size());
    assert(ObjcMethodnamesAddress && ObjcMethodnamesData.size());
    //assert(ObjcCatlistAddress && ObjcCatlistData.size());

    for (unsigned ClasslistIdx = 0; ClasslistIdx < ObjcClasslistData.size(); ClasslistIdx += sizeof(uint64_t)) {
        uint64_t ClassRef = *((uint64_t*)ObjcClasslistData.slice(ClasslistIdx).data());
        ObjcDataStruct_t *ClassData;
        if (ClassRef >= DataAddress) {
            ClassData = (ObjcDataStruct_t*)DataData.slice(ClassRef - DataAddress).data();
        } else {
            assert(ObjcDataAddress <= ClassRef && ClassRef <= ObjcDataAddress + ObjcDataData.size());
            ClassData = (ObjcDataStruct_t*)ObjcDataData.slice(ClassRef - ObjcDataAddress).data();
        }
        DEBUG(errs() << "[+] ObjcDataAddress address: 0x" << utohexstr(ObjcDataAddress) << "\n");
        DEBUG(errs() << "[+] ClassData ISA address: 0x" << utohexstr(ClassData->ISA) << "\n");


        assert(ObjcConstAddress <= ClassData->Data && ClassData->Data <= ObjcConstAddress + ObjcConstData.size());
    
        //try to fix ClassInfo address error(isSwift)
        DEBUG(errs() << "[+] ClassData->Data address: " << utohexstr(ClassData->Data) << "\tObjcConstAddress: " << utohexstr(ObjcConstAddress) << "\tisSwift: " 
            << utohexstr((ClassData->Data & 1)) << "\n");
        
        bool isSwiftClass = (bool)(ClassData->Data & 1);
        ObjcClassInfoStruct_t *ClassInfo = (ObjcClassInfoStruct_t*)ObjcConstData.slice((ClassData->Data  -  ObjcConstAddress), isSwiftClass, true).data();
        //errs() << "[+] ClassInfo size: 0x" << utohexstr(sizeof(*ClassInfo)) << "\n";           
        resolveMethods(ClassInfo, false, isSwiftClass);

        if (ClassData->ISA) {
            //DEBUG(errs() << "[+]ClassData->ISA address: 0x" << utohexstr(ClassData->ISA) << "\tObjcDataAddress: 0x" << utohexstr(ObjcDataAddress) << "\n");
            if (isSwiftClass) {
                ObjcDataStruct_t *ISAData = (ObjcDataStruct_t *) DataData.slice(ClassData->ISA - DataAddress).data();
                //errs() << "ISA: " << utohexstr(ISAData->ISA) << "\n";

                ObjcClassInfoStruct_t *ISAClassInfo = (ObjcClassInfoStruct_t*)ObjcConstData.slice(ISAData->Data - ObjcConstAddress).data();
                DEBUG(errs() << "[+]ISAClassInfo->BaseMethods address: "<< utohexstr(ISAClassInfo->BaseMethods) << "\n");
                DEBUG(errs() << "[+]ISAClassInfo->Name: 0x" << utohexstr(ISAClassInfo->Name) << "\n");
                //(errs() << getClassName(ObjcClassnamesData, ObjcClassnamesAddress, ISAClassInfo->Name) << "\n");
                resolveMethods(ISAClassInfo, true, true);
            } else{
                ObjcDataStruct_t *ISAData = (ObjcDataStruct_t *) ObjcDataData.slice(ClassData->ISA - ObjcDataAddress).data();
                DEBUG(dbgs() << "ISA: " << utohexstr(ISAData->ISA) << "\n");

                ObjcClassInfoStruct_t *ISAClassInfo = (ObjcClassInfoStruct_t*)ObjcConstData.slice(ISAData->Data - ObjcConstAddress).data();
                //errs() << utohexstr(ISAClassInfo->BaseMethods) << "\n";
                DEBUG(errs() << "[+]ISAClassInfo->Name: 0x" << utohexstr(ISAClassInfo->Name) << "\n");

                //errs() << getClassName(ObjcClassnamesData, ObjcClassnamesAddress, ISAClassInfo->Name) << "\n";
                resolveMethods(ISAClassInfo, true, false);
            }
        }
    }
    if (ObjcCatlistData.size()) {
        for (unsigned CatlistIdx = 0; CatlistIdx < ObjcCatlistData.size(); CatlistIdx += sizeof(uint64_t)) {
            uint64_t CatRef = *((uint64_t*)ObjcCatlistData.slice(CatlistIdx).data());

            assert(ObjcConstAddress <= CatRef && CatRef <= ObjcConstAddress + ObjcConstData.size());

            ObjcCatInfoStruct_t *catInfo = (ObjcCatInfoStruct_t*)ObjcConstData.slice(CatRef - ObjcConstAddress).data();
            ObjcDataStruct_t *ClassData = nullptr;
            ObjcClassInfoStruct_t *ClassInfo = nullptr;
            bool isSwiftClass = false;
            if (catInfo->Class) {
                ClassData = (ObjcDataStruct_t *) ObjcDataData.slice(catInfo->Class - ObjcDataAddress).data();
                //try to fix ClassInfo address error(isSwift)
                DEBUG(errs() << "[+] Castlist ClassData->Data address: " << utohexstr(ClassData->Data) << "\n");
                isSwiftClass = (bool)(ClassData->Data & 1);
                ClassInfo = (ObjcClassInfoStruct_t*)ObjcConstData.slice(ClassData->Data  -  ObjcConstAddress, isSwiftClass, true).data();
            }
            resolveMethods(catInfo, true, CatRef, ClassInfo, isSwiftClass);
        }
    }
}

//Objective-C class name
StringRef ObjectiveCFile::getClassName(ArrayRef<uint8_t> &ObjcClassnames, uint64_t ObjcClassNamesAddress,
                                         uint64_t Address) {
    if(!(ObjcClassNamesAddress <= Address && Address <= ObjcClassNamesAddress + ObjcClassnames.size())){
        DEBUG(errs() << "[+] assert failed:\n\tObjcClassNamesAddress: 0x" << utohexstr(ObjcClassnamesAddress) 
            << "\n\tObjcClassNames size: 0x" << utohexstr(ObjcClassnames.size()) << "\n\tAddress: 0x" << utohexstr(Address) << "\n");
    }
    StringRef s((char*)ObjcClassnames.slice(Address - ObjcClassNamesAddress).data());
    return s;
}

//Swift class name
StringRef ObjectiveCFile::getClassName(ArrayRef<uint8_t> &cStrings, uint64_t cStringsAddress,
                                         uint64_t Address, bool isSwiftClass) {
    if(!(cStringsAddress <= Address && Address <= cStringsAddress + cStrings.size())){
        DEBUG(errs() << "[+] assert failed:\n\tcStringsAddress: 0x" << utohexstr(cStringsAddress) 
            << "\n\tObjcClassNames size: 0x" << utohexstr(cStrings.size()) << "\n\tAddress: 0x" << utohexstr(Address) << "\n");
    }
    StringRef s((char*)cStrings.slice(Address - cStringsAddress).data());
    return s;
}

StringRef ObjectiveCFile::getMethodName(ArrayRef<uint8_t> &ObjcMethodnames, uint64_t ObjcMethodnamesAddress,
                                          uint64_t Address) {
    assert(ObjcMethodnamesAddress <= Address && Address <= ObjcMethodnamesAddress + ObjcMethodnames.size());
    //errs() << "[+]Address: 0x" << utohexstr(Address) << "\tAddress - ObjcMethodnamesAddress: " 
    //    << (Address - ObjcMethodnamesAddress) << "\tObjcMethodnames.size(): 0x" << utohexstr(ObjcMethodnames.size()) << "\n";
    StringRef s((char*)ObjcMethodnames.slice(Address - ObjcMethodnamesAddress).data());
    return s;
}

void ObjectiveCFile::resolveMethods(ObjcClassInfoStruct_t *ClassInfo, bool ClassMethods, bool isSwiftClass) {
    if (!ClassInfo->BaseMethods){
        return;
    }

    StringRef ClassName;
    if(isSwiftClass) {
        ClassName = getClassName(cStringsData, cStringsAddress, ClassInfo->Name, true);
    }
    else{
        ClassName = getClassName(ObjcClassnamesData, ObjcClassnamesAddress, ClassInfo->Name);
    }

    if (ClassName == "LastPassModel") {
        assert(true);
    }
    ObjcMethodListHeader_t *MethodlistHeader = (ObjcMethodListHeader_t*)ObjcConstData.slice(ClassInfo->BaseMethods - ObjcConstAddress).data();
    ObjcMethodListEntry_t *MethodlistEntry = (ObjcMethodListEntry_t*)ObjcConstData.slice(ClassInfo->BaseMethods - ObjcConstAddress + sizeof(ObjcMethodListHeader_t)).data();
    for (unsigned MethodIdx = 0; MethodIdx < MethodlistHeader->Count; ++MethodIdx) {
        if (!MethodlistEntry[MethodIdx].Implementation)
            continue;
        StringRef Methodname = getMethodName(ObjcMethodnamesData, ObjcMethodnamesAddress, MethodlistEntry[MethodIdx].Name);
        if (Methodname == "notifyInAppPurchasingEnabledChanged") {
            assert(true);
        }
        std::string N = ((ClassMethods ? "+[" : "-[") + ClassName + " " + Methodname + "]").str();
        // errs() << utohexstr(MethodlistEntry[MethodIdx].Implementation) << ": " << N << "\n";
        FunctionNames.insert(std::pair<uint64_t, std::string>(MethodlistEntry[MethodIdx].Implementation, N));
    }
    //errs() << "[+]resolveMethods end.\n";
}

void ObjectiveCFile::resolveMethods(ObjcCatInfoStruct_t *CatInfo, bool ClassMethods, uint64_t CatInfoAddress, ObjcClassInfoStruct_t *ClassInfo, bool isSwiftClass) {
    StringRef ClassName;
    if (CatInfoAddress == 0x1003a28d0) {
        assert(true);
    }

    DEBUG(dbgs() << "[+]CatInfo->Class address: 0x" << utohexstr(CatInfo->Class));
    if (CatInfo->Class) {
        assert(ClassInfo && ClassInfo->Name);
        DEBUG(dbgs() << "[+]CatInfo->Name address: 0x" << utohexstr(CatInfo->Name));
        StringRef ClassName;
        if(isSwiftClass) {
            ClassName = getClassName(cStringsData, cStringsAddress, ClassInfo->Name, true);
        }
        else{
            ClassName = getClassName(ObjcClassnamesData, ObjcClassnamesAddress, ClassInfo->Name);
        }
    } else {
        //Pointer do function name is not set yet -> binding info
        ClassName = getClassName(CatInfoAddress + 8);
        if (ClassName.startswith("_OBJC_CLASS_$_")) {
            ClassName = ClassName.substr(strlen("_OBJC_CLASS_$_"));
        }
    }

    if (CatInfo->InstaceMethods) {
        ObjcMethodListHeader_t *MethodlistHeader = (ObjcMethodListHeader_t *) ObjcConstData.slice(
                CatInfo->InstaceMethods - ObjcConstAddress).data();
        ObjcMethodListEntry_t *MethodlistEntry = (ObjcMethodListEntry_t *) ObjcConstData.slice(
                CatInfo->InstaceMethods - ObjcConstAddress + sizeof(ObjcMethodListHeader_t)).data();
        for (unsigned MethodIdx = 0; MethodIdx < MethodlistHeader->Count; ++MethodIdx) {
            if (!MethodlistEntry[MethodIdx].Implementation)
                continue;
            StringRef Methodname = getMethodName(ObjcMethodnamesData, ObjcMethodnamesAddress,
                                                 MethodlistEntry[MethodIdx].Name);
            StringRef N = ("-[" + ClassName + " " + Methodname + "]").str();
            // errs() << utohexstr(MethodlistEntry[MethodIdx].Implementation) << ": " << N << "\n";
            FunctionNames.insert(std::pair<uint64_t, std::string>(MethodlistEntry[MethodIdx].Implementation, N.str()));
        }
    }

    if (CatInfo->ClassMethods) {
        ObjcMethodListHeader_t *MethodlistHeader = (ObjcMethodListHeader_t *) ObjcConstData.slice(
                CatInfo->ClassMethods - ObjcConstAddress).data();
        ObjcMethodListEntry_t *MethodlistEntry = (ObjcMethodListEntry_t *) ObjcConstData.slice(
                CatInfo->ClassMethods - ObjcConstAddress + sizeof(ObjcMethodListHeader_t)).data();
        for (unsigned MethodIdx = 0; MethodIdx < MethodlistHeader->Count; ++MethodIdx) {
            if (!MethodlistEntry[MethodIdx].Implementation)
                continue;
            StringRef Methodname = getMethodName(ObjcMethodnamesData, ObjcMethodnamesAddress,
                                                 MethodlistEntry[MethodIdx].Name);
            StringRef N = ("+[" + ClassName + " " + Methodname + "]").str();
            // errs() << utohexstr(MethodlistEntry[MethodIdx].Implementation) << ": " << N << "\n";
            FunctionNames.insert(std::pair<uint64_t, std::string>(MethodlistEntry[MethodIdx].Implementation, N.str()));
        }
    }
}

StringRef ObjectiveCFile::getClassName(uint64_t Pointer) {
    ArrayRef<uint8_t> BindOpcodes = MachO->getDyldInfoBindOpcodes();

    uint64_t SegmentNo = 0;
    int64_t Offset = 0;
    StringRef SymbolName;

    for (unsigned Idx = 0; Idx < BindOpcodes.size();) {
        if (Idx == 283) {
            assert(true);
        }
        unsigned Opcode = *BindOpcodes.slice(Idx).data();

        if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_DYLIB_ORDINAL_IMM) {
            unsigned Imm = Opcode & 0xF;
            Idx++;
        } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM) {
            Idx++;
            SymbolName = (const char*)BindOpcodes.slice(Idx).data();
            Idx += SymbolName.size() + 1;
        } else if (Opcode == (MachO::BIND_OPCODE_SET_TYPE_IMM | 0x1)) {
            //Set type pointer
            Idx++;
        } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB) {
            SegmentNo = Opcode & 0xF;
            Idx++;
            unsigned n;
            Offset = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_DO_BIND) {
            MachO::segment_command_64 Segment = getSegment(SegmentNo);
            if (Segment.vmaddr + Offset == Pointer) {
                return SymbolName;
            }
            //FIXME: is this add correct???
            Offset += 8;
            Idx++;
        } else if (Opcode == MachO::BIND_OPCODE_ADD_ADDR_ULEB) {
            Idx++;
            unsigned n;
            Offset += decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
        } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED) {
            unsigned Scaled = (Opcode & 0xF) * 8;
            if (getSegment(SegmentNo).vmaddr + Offset == Pointer) {
                return SymbolName;
            }
//            addClass(SymbolName, getSegment(SegmentNo).vmaddr + Offset);
            //FIXME: again the add of 8
            Offset += Scaled + 8;
            Idx++;
        } else if (Opcode == MachO::BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB) {
            //TODO:
            Idx++;
            unsigned n;
            uint64_t Count = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
            n = 0;
            uint64_t Skip = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            for (unsigned i = 0; i < Count; ++i) {
//                addClass(SymbolName, getSegment(SegmentNo).vmaddr + Offset);
                if (getSegment(SegmentNo).vmaddr + Offset == Pointer) {
                    return SymbolName;
                }
                Offset += 8 + Skip;
            }
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB) {
//            addClass(SymbolName, getSegment(SegmentNo).vmaddr + Offset);
            if (getSegment(SegmentNo).vmaddr + Offset == Pointer) {
                return SymbolName;
            }
            Idx++;
            unsigned n;
            Offset += decodeULEB128(BindOpcodes.slice(Idx).data(), &n) + 8;
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_DONE) {
            break;
        } else if (Opcode == MachO::BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB) {
            Idx++;
            unsigned n;
            uint64_t dylib = decodeULEB128(BindOpcodes.slice(Idx).data(), &n) + 8;
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_SET_ADDEND_SLEB) {
            Idx++;
            unsigned n;
            uint64_t addend = decodeSLEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
        } else {
            errs() << "0X" << utohexstr(Idx) << "\n";
            errs() << "0x" << utohexstr(Opcode) << "\n";
            assert(false);
        }
    }
    llvm_unreachable("");
}

MachO::segment_command_64 ObjectiveCFile::getSegment(uint64_t SegmentNo) {
    uint64_t Idx = 0;
    for(object::MachOObjectFile::load_command_iterator L_it = MachO->begin_load_commands();
        L_it != MachO->end_load_commands(); ++L_it) {
        if (L_it->C.cmd == MachO::LC_SEGMENT_64) {
            if (Idx == SegmentNo) {
                return MachO->getSegment64LoadCommand(*L_it);
            } else {
                Idx++;
            }
        }
    }
    llvm_unreachable("Cant find segment");
};
