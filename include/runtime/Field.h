#include "ClassBase.h"
#include "../types/General.h"
#include <string>
#include <map>
using std::string;
#ifndef FIELD_ENTRY_DEF
#define FIELD_ENTRY_DEF
class FieldEntry {
    string name;
    string descriptor;
    string name_and_descriptor;

    UShort byte;

public:
    bool ACC_PUBLIC;
    bool ACC_PRIVATE;
    bool ACC_PROTECTED;
    bool ACC_STATIC;
    bool ACC_FINAL;
    bool ACC_VOLATILE;
    bool ACC_TRANSIENT;
    bool ACC_SYNTHETIC;
    bool ACC_ENUM;
    
    FieldEntry() {}
    FieldEntry(FIELDINFO* pfi, class_attribute* pkl) ;
    /* {
        ACC_PUBLIC = pfi->ACC_PUBLIC;
        ACC_PRIVATE = pfi->ACC_PRIVATE;
        ACC_PROTECTED = pfi->ACC_PROTECTED;
        ACC_STATIC = pfi->ACC_STATIC;
        ACC_FINAL = pfi->ACC_FINAL;
        ACC_SYNCHRONIZE = pfi->ACC_SYNCHRONIZE
        ACC_BRIDGE = pfi->ACC_BRIDGE;
        ACC_VARARGS = pfi->ACC_VARARGS;
        ACC_NATIVE = pfi->ACC_NATIVE;
        ACC_ABSTRACT = pfi->ACC_ABSTRACT;
        ACC_STRICTFP = pfi->ACC_STRICTFP;
        ACC_SYNTHETIC = pfi->ACC_SYNTHETIC; 

        auto info = pfi->get_info(pkl);

        name = info.first;
        descriptor = info.second;
        byte = resolve(descriptor);
        name_and_descriptor = name + descriptor;
    } 
    */

    UShort GetByte(); 

    string GetName();
    string GetDescriptor(); 
    string GetNameAndDescriptor(); 
    friend bool MakeFieldTable(std::map<string, int> &ftp, pClass *pkl);
};

UShort field_resolve(string dp); 
bool MakeFieldTable(std::map<string, int> &ftp, pClass *pkl);
#endif
