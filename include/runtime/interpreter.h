#include <iostream>
#include <string>
#include "javap.h"
#include "../types/ClassLoader.h"
#include "../types/ClassFile.h"
#include "MethodPool.h"

#define MAX_THREAD_STACK 10000

using namespace std;
class VirtualTable;
class StaticMethodTable;
class MethodEntry;
class FieldEntry;
class ConstantPool;


class StackFrame{
public:
    uint start_pos;             //在线程私有栈的起始位置
    uint end_pos;               //结束位置的下一位
    uint frame_length;          //栈帧的长度
    uint return_address;        //bytecode返回的pc值
    uint local_variable_table;  //本地变量表起始位置
    uint operand_stack_start;   //操作数栈起始位置
    uint stack_top;             //栈顶位置
    uint dynamic_linking;       //存放指向this的指针的位置
    uint max_locals;
    uint max_stack;
    bool* is_obj;
    StackFrame* caller;
    StackFrame(uint max_locals, uint max_stack, StackFrame* _caller);
};

int parse_param(StackFrame* frame, string type);












