#include "../include/types/ClassLoader.h"
//#include "../include/types/ClassFile.h"
#include <iostream>

// ClassLoader::Loader 流程
// 1)  将类信息文件的常量池信息载入常量池
// 2)  将方法载入方法区,并处理出非虚函数表和虚函数表
// 3)  将字段表信息载入方法区
// 4)  创建该类对应的pClass对象，设置堆中句柄对象和实例对象的映射关系

namespace ClassLoader {

    std::map<std::string, int> nameMap;
    pClass* arrLoadedClass[MAX_LOADED_CLASSNUM];  

    std::string class_path;

    void SetPath(string cp) {class_path = cp;}

    int LoadClass(string class_name) { //class_attribute* pkla) {
        int res_pos = -1;
        pClass *pkl = NULL;
        for (int i = 0; i < ClassLoader::MAX_LOADED_CLASSNUM; i++)
            if (ClassLoader::arrLoadedClass[i] == NULL) { 
                res_pos = i;
                break;
            }

        if (res_pos < 0) {
            throw "ClassLoader can't load any more class right now!";
            return 0;
        }

        class_attribute* pkla = new class_attribute(class_name.c_str());
        pkl = new pClass(pkla);

        ClassLoader::arrLoadedClass[res_pos] = pkl;
        ClassLoader::nameMap[std::string(class_name)] = res_pos;
        pkl->set_loaded();

        return 1;
    }

    int LoadClass(string class_name, bool chk_path) { //class_attribute* pkla) {
        int res_pos = -1;
        pClass *pkl = NULL;
        for (int i = 0; i < ClassLoader::MAX_LOADED_CLASSNUM; i++)
            if (ClassLoader::arrLoadedClass[i] == NULL) { 
                res_pos = i;
                break;
            }

        if (res_pos < 0) {
            throw "ClassLoader can't load any more class right now!";
            return 0;
        }

        class_attribute* pkla = new class_attribute((class_path+class_name+".class").c_str());
        pkl = new pClass(pkla);

        ClassLoader::arrLoadedClass[res_pos] = pkl;
        ClassLoader::nameMap[std::string(class_name)] = res_pos;
        pkl->set_loaded();

        return 1;
    }

    pClass* findLoadedClass(const char* class_name) {
        if (ClassLoader::nameMap.count(class_name)) 
            return ClassLoader::arrLoadedClass[ClassLoader::nameMap[class_name]];
        return NULL;
    }
    
    pClass* findLoadedClass(std::string class_name) {
        if (ClassLoader::nameMap.count(class_name)) 
            return ClassLoader::arrLoadedClass[ClassLoader::nameMap[class_name]];
        return NULL;
    }
}
