#ifndef LLVM_OBJECTIVECFILE_H
#define LLVM_OBJECTIVECFILE_H

#include "llvm/Object/MachO.h"

#include <map>
#include <string>

namespace llvm {

    class ObjectiveCFile {
    public:
        ObjectiveCFile(object::MachOObjectFile *MachO) : MachO(MachO) {
            resolveMethods();
        };

        struct ObjcClass_t {
            StringRef ClassName;
        };
        typedef struct ObjcClass_t ObjcClass_t;

        struct ObjcMethod_t {
            ObjcClass_t Class;
            StringRef MethodName;
            bool isClassMethod;
            uint64_t IMP;
        };
        typedef struct ObjcMethod_t ObjcMethod_t;

        std::map<uint64_t, std::string> getFunctionNames() {
            return FunctionNames;
        };
        std::string getFunctionName(uint64_t Address) {
            return FunctionNames[Address];
        }
    private:
        struct ObjcDataStruct_t {
            uint64_t ISA;
            uint64_t Super;
            uint64_t Cache;
            uint64_t VTable;
            uint64_t Data;
        };
        typedef struct ObjcDataStruct_t ObjcDataStruct_t;

        struct ObjcClassInfoStruct_t {
            uint32_t Flags;
            uint32_t InstanceStart;
            uint32_t InstanceSize;
            uint32_t Reserved;
            uint64_t IVarLayout;
            uint64_t Name;
            uint64_t BaseMethods;
            uint64_t BaseProtocols;
            uint64_t IVars;
            uint64_t WeakIVarLayout;
            uint64_t BaseProperties;
        };
        typedef struct ObjcClassInfoStruct_t ObjcClassInfoStruct_t;

        typedef struct {
            uint64_t Name;
            uint64_t Class;
            uint64_t InstaceMethods;
            uint64_t ClassMethods;
        } ObjcCatInfoStruct_t;

        struct ObjcMethodListHeader_t {
            uint32_t EntrySize;
            uint32_t Count;
        };
        typedef struct ObjcMethodListHeader_t ObjcMethodListHeader_t;

        struct ObjcMethodListEntry_t {
            uint64_t Name;
            uint64_t Types;
            uint64_t Implementation;
        };


        object::MachOObjectFile *MachO;

        uint64_t ObjcClasslistAddress = 0;
        ArrayRef<uint8_t> ObjcClasslistData;

        uint64_t ObjcDataAddress = 0;
        ArrayRef<uint8_t> ObjcDataData;

        uint64_t DataAddress = 0;
        ArrayRef<uint8_t> DataData;

        uint64_t ObjcConstAddress = 0;
        ArrayRef<uint8_t> ObjcConstData;

        uint64_t ObjcMethodnamesAddress = 0;
        ArrayRef<uint8_t> ObjcMethodnamesData;

        uint64_t ObjcClassnamesAddress = 0;
        ArrayRef<uint8_t> ObjcClassnamesData;

        //__cstring section strings ,to get Swift classname.
        uint64_t cStringsAddress = 0;
        ArrayRef<uint8_t> cStringsData;

        uint64_t ObjcCatlistAddress = 0;
        ArrayRef<uint8_t> ObjcCatlistData;

        std::map<uint64_t, std::string> FunctionNames;

        void resolveMethods();

        void resolveMethods(ObjcClassInfoStruct_t *ClassInfo, bool ClassMethods, bool isSwiftClass);
        void resolveMethods(ObjcCatInfoStruct_t *CatInfo, bool ClassMethods, uint64_t CatInfoAddress, ObjcClassInfoStruct_t *ClassInfo, bool isSwiftClass);

        StringRef getClassName(ArrayRef<uint8_t> &ObjcClassnames, uint64_t ObjcClassNamesAddress, uint64_t Address);
        StringRef getClassName(ArrayRef<uint8_t> &cStrings, uint64_t cStringsAddress, uint64_t Address, bool isSwiftClass);
        
        StringRef getMethodName(ArrayRef<uint8_t> &ObjcMethodnames, uint64_t ObjcMethodnamesAddress, uint64_t Address);

        StringRef getClassName(uint64_t Pointer);
        MachO::segment_command_64 getSegment(uint64_t SegmentNo);
    };

}

#endif //LLVM_OBJECTIVECFILE_H
