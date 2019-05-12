#include <iostream>
#include "javap.h"
#include "ClassLoader.h"
#include "ClassFile.h"
#include "MethodPool.h"

#define MAX_THREAD_STACK 10000

class VirtualTable;
class StaticMethodTable;
class MethodEntry;
class FieldEntry;
class ConstantPool;

using UShort = unsigned short;
using UInt = unsigned short;

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
    StackFrame* caller;
    StackFrame(uint max_locals, uint max_stack, StackFrame* _caller);
};

