#include "../include/types/ClassFile.h"
#include "../include/types/ClassLoader.h"
#include <cstdio>
int main(int argc, char**argv) {
    pClass* pkl;
    string test = string(argv[1]);
    int temp_char = 0;
    cout << test << endl;   
    for (int k = test.length()-1; k>=0; k--)
        if (test[k] == '/') {
            temp_char = k;
            break;
        } 
    cout << test.substr(0, temp_char+1) << endl;
    ClassLoader::SetPath(test.substr(0, temp_char+1));
    ClassLoader::LoadClass(argv[1]);
    std::string a = argv[1];
    cout << "=============================" << endl;
    pkl = ClassLoader::findLoadedClass(a);
    for (int k=1; k<pkl->constant_pool_count; k++) {
        cout << k<<":["<<pkl->pcp->GetConstantPoolItem(k)->GetInstanceType() << endl;
        if (pkl->pcp->GetConstantPoolItem(k)->GetInstanceType() == CONS_CLASS)
            cout << *(string*)pkl->pcp->GetConstantPoolItem(k)->GetValue()<<","<<endl;
        else
            cout <<pkl->pcp->GetConstantPoolItem(k)->GetNameAndType() << endl;
    }
    cout << "============STARS<>L=========" << endl;
    auto g = pkl->vtp->GetEntryList();
    for (int k =0; k <g.size(); k++) {
        cout << g[k]->GetNameAndDescriptor() << "  [ " << g[k] << endl;
    }

    cout << "============STARS<>PAR=========" << endl;
    auto gg = pkl->pFatherClass->vtp->GetEntryList();
    for (int k =0; k <gg.size(); k++) {
        cout << gg[k]->GetNameAndDescriptor() << "  [ " << gg[k]<< endl;
    }

    return 0;
}
