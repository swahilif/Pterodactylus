// 这个文件主要实现将静态数据结构加载为虚拟机的运行时数据结构
// 并处理出每个类对应的字段表和虚函数表,以实现多态功能

#ifndef CLASSLOADER_DEF 
#define CLASSLOADER_DEF 
#include "../types/ClassFile.h"
//#include "General.h"
#include <map>
#include <string>

namespace ClassLoader {

    extern std::map<std::string, int> nameMap;
    
    const int MAX_LOADED_CLASSNUM = 1000;
    extern pClass* arrLoadedClass[MAX_LOADED_CLASSNUM];  
    
    void SetPath(string);
    int LoadClass(string); 
    int LoadClass(string, bool);
    pClass* findLoadedClass(std::string class_name);
}
#endif 

