#include "ClassBase.h"
#include "../types/General.h"
#include <string>
#include <map>
using std::string;

class FieldEntry {
    string name;
    string descriptor;
    string name_and_descriptor;

    UShort byte;

    static UShort resolve(string dp) {
        switch (dp[0]) {
            case 'I':
                return sizeof(int);
            case 'F':
                return sizeof(float);
            case 'L':
                return sizeof(int);
            case 'J':
                return sizeof(long);
            case 'S':
                return sizeof(short);
            case 'Z':
                return sizeof(bool);
            case 'B':
                return sizeof(char);
            case 'C':
                return sizeof(char);
            case 'D':
                return sizeof(double);
            default:
                return 0;
        }
    }
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
};

bool MakeFieldTable(std::map<string, int> &ftp, pClass *pkl);
