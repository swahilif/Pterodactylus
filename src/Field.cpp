#include "../include/runtime/Field.h"
#include "../include/types/ClassFile.h"

UShort field_resolve(string dp) {
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

UShort FieldEntry::GetByte() {return byte;}

string FieldEntry::GetName() {return name;}
string FieldEntry::GetDescriptor() {return descriptor;}
string FieldEntry::GetNameAndDescriptor() {return name_and_descriptor;}

FieldEntry::FieldEntry(FIELDINFO* pfi, class_attribute* pkl) {
    ACC_PUBLIC = pfi->ACC_PUBLIC;
    ACC_PRIVATE = pfi->ACC_PRIVATE;
    ACC_PROTECTED = pfi->ACC_PROTECTED;
    ACC_STATIC = pfi->ACC_STATIC;
    ACC_FINAL = pfi->ACC_FINAL;

    ACC_SYNTHETIC = pfi->ACC_SYNTHETIC; 

    auto info = pfi->get_info(pkl);

    name = info.first;
    descriptor = info.second;
    byte = field_resolve(descriptor);
    name_and_descriptor = name + descriptor;
}

bool MakeFieldTable(std::map<string, int> &ftp, pClass *pkl) {
    if (pkl == NULL or pkl->arrField == NULL) return false;
    for (int k = pkl->field_count - 1; k >= 0; k--) {
        if (pkl->arrField[k] != NULL) {
            ftp[pkl->arrField[k]->GetNameAndDescriptor()] = k;
        }
    }
    pkl->byteGrad[0] = 0;
    for (int k = 1; k < pkl->field_count; k++) {
        pkl->byteGrad[k] = pkl->byteGrad[k-1] + pkl->arrField[k-1]->GetByte();
    }
    return true;
}
