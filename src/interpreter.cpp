#include "../include/runtime/interpreter.h"
#include "../include/types/ClassLoader.h"
#include "../include/runtime/ConstantPool.h"
#include "../include/types/Object.h"
#include "../include/runtime/StaticMethodTable.h"
#include "../include/runtime/HeapManager.hpp"
#include <getopt.h>

#define DEBUG

/*
static struct option long_options[] = {
    {"file", required_argument, NULL, }
}
 */

StackFrame::StackFrame(uint _max_locals, uint _max_stack, StackFrame* call){
    if (call != NULL)
        start_pos = call->end_pos;
    else
        start_pos = -1;
    is_obj = new bool[_max_locals];
    memset(is_obj, 0, _max_locals);
    return_address = start_pos + 1;
    dynamic_linking = start_pos + 2;
    local_variable_table = start_pos + 3;
    operand_stack_start = start_pos + 3 + _max_locals;
    stack_top = operand_stack_start - 1; //初始状态是指向起始点-1位的
    end_pos = operand_stack_start + _max_stack;
    frame_length = end_pos - start_pos;
    max_locals = _max_locals;
    max_stack = _max_stack;
    caller = call;
}

StackFrame* set_new_stack_frame(MethodEntry* method, StackFrame* top){
    PureCode* code = GenCodePool.Access(method->GetMethodResPos());
    ushort max_locals = code->GetMaxLocals();
    ushort max_stack = code->GetMaxStack();
    StackFrame* new_frame = new StackFrame(max_locals, max_stack, top);
    return new_frame;
}

int parse_param(string type){
#ifdef DEBUG
    cout << type << endl;
#endif
    int start_pos = type.find('(');
    int end_pos = type.find(')');
    string params = type.substr(start_pos+1, end_pos-1);
    int count = 0;
    int pos = 0;
    int length = params.length();
    while(pos < length){
        if (params[pos] != 'L' && params[pos] != '['){
            count ++;
            pos ++;
        }
        else if (params[pos] == 'L'){
            pos += params.substr(pos, params.length()).find(';') + 1;
            count ++;
        }
        else if (params[pos] == '['){
            while (params[pos] == '[')
                pos += 1;
        }
    }
    return count;
}

void interpret(MethodEntry* method, pClass* cl, StackFrame* frame, ulong* thread_stack){
    PureCode* code = GenCodePool.Access(method->GetMethodResPos());
    uint code_length = code->GetCodeLength();
#ifdef DEBUG
    cout << "Code length: " << code_length << endl;
#endif
    int count = 0;
    uchar tmp;
    while(count < code_length){
        uchar bytecode = code->GetNextCode();
#ifdef DEBUG
        printf("Code: %x\n", int(bytecode));
#endif
        switch(bytecode){
            // dup
            case (0x59):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->stack_top];
                frame->stack_top++;
                break;
            }
            // dup2
            case (0x5c):{
                thread_stack[frame->stack_top+2] = thread_stack[frame->stack_top];
                thread_stack[frame->stack_top+1] = thread_stack[frame->stack_top-1];
                frame->stack_top+=2;
                break;
            }
            // ldc
            case (0x12):{
                
                break;
            }
            // bipush [byte1]
            case (0x10):{
                tmp = code->GetNextCode();
#ifdef DEBUG
                cout << "Bipush " << int(tmp) << endl;
#endif
                thread_stack[++frame->stack_top] = long(tmp);
                cout << "Stack top: " << thread_stack[frame->stack_top] << " Second one: " << thread_stack[frame->stack_top-1] << endl;
                break;
            }
            // iconst_0
            case (0x03):{
                thread_stack[frame->stack_top+1] = 0;
                frame->stack_top ++;
                break;
            }
            // iconst_1
            case (0x04):{
                thread_stack[frame->stack_top+1] = 1;
                frame->stack_top ++;
                break;
            }
            // iconst_2
            case (0x05):{
                thread_stack[frame->stack_top+1] = 2;
                frame->stack_top ++;
                break;
            }
            // iconst_3
            case (0x06):{
                thread_stack[frame->stack_top+1] = 3;
                frame->stack_top ++;
                break;
            }
            // iconst_4
            case (0x07):{
                thread_stack[frame->stack_top+1] = 4;
                frame->stack_top ++;
                break;
            }
            // iconst_5
            case (0x08):{
                thread_stack[frame->stack_top+1] = 5;
                frame->stack_top ++;
                break;
            }
            // iinc [index const]
            case (0x84):{
                int index = code->GetNextCode();
                int c = code->GetNextCode();
                thread_stack[frame->local_variable_table+index] += c;
                break;
            }
            // iadd
            case (0x60):{
                long res = thread_stack[frame->stack_top] + thread_stack[frame->stack_top-1];
                thread_stack[--frame->stack_top] = res;
                break;
            }
            // imul
            case (0x68):{
                long res = thread_stack[frame->stack_top] * thread_stack[frame->stack_top-1];
                thread_stack[--frame->stack_top] = res;
                break;
            }
            // idiv
            case (0x6c):{
                long res = thread_stack[frame->stack_top-1] / thread_stack[frame->stack_top];
                cout << "div result:" << res << endl;
                thread_stack[--frame->stack_top] = res;
                break;
            }
            // isub
            case (0x64):{
                long res = thread_stack[frame->stack_top-1] - thread_stack[frame->stack_top];
                thread_stack[--frame->stack_top] = res;
                break;
            }
            // aload_0
            case (0x2a):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table];
                frame->stack_top++;
                break;
            }
            // aload_1
            case (0x2b):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+1];
                frame->stack_top++;
                break;
            }
            // aload_2
            case (0x2c):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+2];
                frame->stack_top++;
                break;
            }
            // aload_3
            case (0x2d):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+3];
                frame->stack_top++;
                break;
            }
            // iload [index]
            case (0x15):{
                tmp = code->GetNextCode();
                thread_stack[++frame->stack_top] = thread_stack[frame->local_variable_table+tmp];
                break;
            }
            // iload_0
            case (0x1a):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table];
                frame->stack_top++;
                break;
            }
            // iload_1
            case (0x1b):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+1];
                frame->stack_top++;
                break;
            }
            // iload_2
            case (0x1c):{
                cout << "ILOAD2: " << thread_stack[frame->local_variable_table+2] << endl;
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+2];
                frame->stack_top++;
                break;
            }
            // iload_3
            case (0x1d):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+3];
                frame->stack_top++;
                break;
            }
            // lload_0
            case (0x1e):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table];
                frame->stack_top++;
                break;
            }
            // lload_1
            case (0x1f):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+1];
                frame->stack_top++;
                break;
            }
            // lload_2
            case (0x20):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+2];
                frame->stack_top++;
                break;
            }
            // lload_3
            case (0x21):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->local_variable_table+3];
                frame->stack_top++;
                break;
            }
            // astore [indexbyte]
            case (0x3a):{
                tmp = code->GetNextCode();
#ifdef DEBUG
                cout << "Slot index: " << int(tmp) << endl;
#endif
                Object* old_obj = (Object*)thread_stack[frame->local_variable_table+tmp];
                if (old_obj != NULL)
                    old_obj->header->changeCnt(-1);
                Object* new_obj = (Object*)thread_stack[frame->stack_top];
                new_obj->header->changeCnt(1);
                thread_stack[frame->local_variable_table+tmp] = thread_stack[frame->stack_top];
                frame->stack_top--;
                frame->is_obj[tmp] = true;
                break;            }
            // astore_0
            case (0x4b):{
                Object* old_obj = (Object*)thread_stack[frame->local_variable_table];
                if (old_obj != NULL)
                    old_obj->header->changeCnt(-1);
                Object* new_obj = (Object*)thread_stack[frame->stack_top];
                new_obj->header->changeCnt(1);
                thread_stack[frame->local_variable_table] = thread_stack[frame->stack_top];
                frame->stack_top--;
                frame->is_obj[0] = true;
                break;
            }
            // astore_1
            case (0x4c):{
                Object* old_obj = (Object*)thread_stack[frame->local_variable_table+1];
                if (old_obj != NULL){
                    old_obj->header->changeCnt(-1);
#ifdef DEBUG
                    cout << "Old object got replaced! Its new count is: " << old_obj->header->getCount() << endl;
#endif
                }
                Object* new_obj = (Object*)thread_stack[frame->stack_top];
                new_obj->header->changeCnt(1);
#ifdef DEBUG
                cout << "New object " << (long)new_obj << " got updated! Its new count is: " << new_obj->header->getCount() << endl;
#endif
                thread_stack[frame->local_variable_table+1] = thread_stack[frame->stack_top];
                frame->stack_top--;
                frame->is_obj[1] = true;
                break;
            }
            // astore_2
            case (0x4d):{
                Object* old_obj = (Object*)thread_stack[frame->local_variable_table+2];
                if (old_obj != NULL){
                    old_obj->header->changeCnt(-1);
#ifdef DEBUG
                    cout << "Old object got replaced! Its new count is: " << old_obj->header->getCount() << endl;
#endif
                }
                Object* new_obj = (Object*)thread_stack[frame->stack_top];
                new_obj->header->changeCnt(1);
#ifdef DEBUG
                cout << "New object " << (long)new_obj << " got updated! Its new count is: " << new_obj->header->getCount() << endl;
#endif
                thread_stack[frame->local_variable_table+2] = thread_stack[frame->stack_top];
                frame->stack_top--;
                frame->is_obj[2] = true;
                break;
            }
            // astore_3
            case (0x4e):{
                Object* old_obj = (Object*)thread_stack[frame->local_variable_table+3];
                cout << "Address of the old object: " << (long)old_obj << endl;
                if (old_obj != NULL){
                    old_obj->header->changeCnt(-1);
#ifdef DEBUG
                    cout << "Old object got replaced! Its new count is: " << old_obj->header->getCount() << endl;
#endif
                }
                Object* new_obj = (Object*)thread_stack[frame->stack_top];
                new_obj->header->changeCnt(1);
#ifdef DEBUG
                cout << "New object " << (long)new_obj << " got updated! Its new count is: " << new_obj->header->getCount() << endl;
#endif
                thread_stack[frame->local_variable_table+3] = thread_stack[frame->stack_top];
                frame->stack_top--;
                frame->is_obj[3] = true;
                break;
            }
            // istore [index]
            case (0x36):{
                tmp = code->GetNextCode();
                thread_stack[frame->local_variable_table+tmp] = thread_stack[frame->stack_top--];
            }
            // istore_0
            case (0x3b):{
                thread_stack[frame->local_variable_table] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // istore_1
            case (0x3c):{
                thread_stack[frame->local_variable_table+1] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // istore_2
            case (0x3d):{
                thread_stack[frame->local_variable_table+2] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // istore_3
            case (0x3e):{
                thread_stack[frame->local_variable_table+3] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // pop
            case (0x57):{
                frame->stack_top--;
                break;
            }
            // if_icmpeq
            case (0x9f):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                int op1 = thread_stack[frame->stack_top-1];
                int op2 = thread_stack[frame->stack_top];
                frame->stack_top -= 2;
                if (op1 == op2)
                    code->Jump(tmp-3);
                break;
            }
            // if_icmpne
            case (0xa0):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                int op1 = thread_stack[frame->stack_top-1];
                int op2 = thread_stack[frame->stack_top];
                frame->stack_top -= 2;
                if (op1 != op2)
                    code->Jump(tmp-3);
                break;
            }
            // if_icmplt
            case (0xa1):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                int op1 = thread_stack[frame->stack_top-1];
                int op2 = thread_stack[frame->stack_top];
                frame->stack_top -= 2;
                if (op1 < op2)
                    code->Jump(tmp-3);
                break;
            }
            // if_icmple
            case (0xa4):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                int op1 = thread_stack[frame->stack_top-1];
                int op2 = thread_stack[frame->stack_top];
                frame->stack_top -= 2;
                if (op1 <= op2)
                    code->Jump(tmp-3);
                break;
            }
            // if_icmpgt
            case (0xa3):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                int op1 = thread_stack[frame->stack_top-1];
                int op2 = thread_stack[frame->stack_top];
                frame->stack_top -= 2;
                if (op1 > op2)
                    code->Jump(tmp-3);
                break;
            }
            // if_icmpge
            case (0xa2):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                long op2 = (long)thread_stack[frame->stack_top--];
                long op1 = (long)thread_stack[frame->stack_top--];
#ifdef DEBUG
                cout << "Comparing two numbers " << op1 << " and " << op2 << endl;
#endif
                if (op1 >= op2)
                    code->Jump(tmp-3);
                break;
            }
            // goto
            case (0xa7):{
                uchar byte1 = code->GetNextCode();
                uchar byte2 = code->GetNextCode();
                short offset = (short)( (((ushort)byte1) << 8) + (ushort)byte2);
                cout << "offset: " << offset << endl;
                frame->stack_top -= 2;
                code->Jump(offset-3);
                break;
            }
            // ireturn
            case (0xac):{
                cout << "Return Value: " << thread_stack[frame->stack_top] << endl;
                for (int i = 0; i < frame->max_locals; i ++){
                    if (frame->is_obj[i]){

                        cout << "Local Var " << i << " is obj." << endl;
                        cout << "Address: " << thread_stack[frame->local_variable_table+i] << endl;
                        ((Object*)thread_stack[frame->local_variable_table+i])->header->changeCnt(-1);
                    }
                }
                if (frame->caller != NULL)
                    thread_stack[++frame->caller->stack_top] = thread_stack[frame->stack_top];
                return ;
            }
            // areturn
            case (0xb0):{
                for (int i = 0; i < frame->max_locals; i ++){
                    if (frame->is_obj[i]){
                        ((Object*)thread_stack[frame->local_variable_table+i])->header->changeCnt(-1);
                    }
                }
                if (frame->caller != NULL)
                    thread_stack[++frame->caller->stack_top] = thread_stack[frame->stack_top];
                return ;
            }
            // return
            case (0xb1):{
                for (int i = 0; i < frame->max_locals; i ++){
                    if (frame->is_obj[i]){
                        cout << "Local Var " << i << " is obj." << endl;
                        cout << "Address: " << thread_stack[frame->local_variable_table+i] << endl;
                        ((Object*)thread_stack[frame->local_variable_table+i])->header->changeCnt(-1);
                    }
                }
                return ;
            }
            // new [0 index]
            case(0xbb):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp);
                string class_name = *(string*)item->GetValue();
                //class_name = class_name + ".class";
#ifdef DEBUG
                cout << class_name << endl;
#endif
                pClass* new_class = ClassLoader::findLoadedClass(class_name);
                if (!new_class){
                    if(!ClassLoader::LoadClass(class_name, 1))
                        throw "Load Class Failed!";
                    new_class = ClassLoader::findLoadedClass(class_name);
                }
                uint len = new_class->GetByte();
#ifdef DEBUG
                cout << "Length of the new obj: " << len << endl;
#endif
                //Object* new_obj = new Object(new_class, len);
                bool succeed = heap->check_space(len);
                if (!succeed){
                    cout << "Out Of Memory!" << endl;
                    exit(0);
                }
                Object* new_obj = requestObject();
                new_obj->Set(new_class, len);
                cout << "Initial Count: " << new_obj->header->getCount() << endl;
                frame->stack_top++;
                thread_stack[frame->stack_top] = (ulong)new_obj;
#ifdef DEBUG
                cout << "Address: " << (ulong)new_obj << endl;
#endif
                break;
            }
            // getstatic [indexbyte1 indexbyte2]
            case(0xb2):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp);
                item -> resolved(cl->pcp);
                string full_name = item->GetNameAndType();
                cout << "Full Name: " << full_name << endl;
                if (full_name == "java/lang/System.out:Ljava/io/PrintStream;"){
                    //thread_stack[++frame->stack_top] = 1234;
                    break;
                }
                ulong pos = full_name.find('.');
                string class_name = full_name.substr(0, pos);
                pClass* new_class = ClassLoader::findLoadedClass(class_name);
                if (!new_class){
                    if(!ClassLoader::LoadClass(class_name))
                        throw "Load Class Failed!";
                    new_class = ClassLoader::findLoadedClass(class_name);
                }
                //ulong res = (long)new_class->;
                break;
            }
            // getfield [indexbyte1 indexbyte2]
            case(0xb4):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp);
                item -> resolved(cl->pcp);
                string full_name = item->GetNameAndType();
                ulong pos = full_name.find('.');
                string nat = full_name.substr(pos+1, full_name.length() - pos);
#ifdef DEBUG
                cout << "NAT: " << nat << endl;
                cout << "OBJ Address: " << thread_stack[frame->stack_top] << endl;
#endif
                ulong type_pos = nat.find(':');
                string type = nat.substr(type_pos+1, nat.length() - type_pos);
                Object *obj = reinterpret_cast<Object*>(thread_stack[frame->stack_top]);
                ulong res;
                if (type[0] == 'I')
                    res = *(uint*)obj->getField(nat);
                else if (type[0] == 'L')
                    res = *(ulong*)obj->getField(nat);
                frame->stack_top++;
                thread_stack[frame->stack_top] = res;
                break;
            }
            // putfield [indexbyte1 indexbyte2]
            case(0xb5):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp);
                item -> resolved(cl->pcp);
                string full_name = item->GetNameAndType();
                ulong pos = full_name.find('.');
                string nat = full_name.substr(pos+1, full_name.length() - pos);
                ulong type_pos = nat.find(':');
                string type = nat.substr(type_pos+1, nat.length() - type_pos);
                ulong value = thread_stack[frame->stack_top];
                cout << "TYPE: " << type << endl;
                cout << "VALUE: " << value << endl;
                Object *obj = reinterpret_cast<Object*>(thread_stack[frame->stack_top-1]);
                if (type[0] == 'I'){
                    obj->putField(nat, (char*)&value, 4);
                }
                else if (type[0] == 'D')
                    obj->putField(nat, (char*)&value, 8);
                else if (type[0] == 'L'){
                    obj->putField(nat, (char*)&value, 8);
                    Object* tmp = (Object*)*(long*)(obj->getField(nat));
                    cout << "check: " << (long)tmp << " raw: " << thread_stack[frame->stack_top] << endl;
                }
                frame->stack_top -= 2;
                break;
            }
            // invokevirtual [indexbyte1 indexbyte2]
            case(0xb6):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp); // MethodInfo
                string full_name = item->GetNameAndType();
                if (full_name == "java/io/PrintStream.println:(I)V"){
                    cout << (long)thread_stack[frame->stack_top--] << endl;
                    break;
                }
                cout << full_name << endl;
                ulong pos = full_name.find('.');
                string nat = full_name.substr(pos+1, full_name.length() - pos + 1);
                ulong type_pos = nat.find(':');
                string type = nat.substr(type_pos+1, nat.length() - type_pos);
                int param_count = parse_param(type);
#ifdef DEBUG
                cout << "Param COUNT: " << param_count << endl;
#endif
                Object *obj = (Object*)thread_stack[frame->stack_top-param_count];
                cout << "Address: " << (ulong)obj << endl;
                // TO BE MODIFIED
                pClass* new_cl = obj->getClass();
                if (!new_cl){
                    throw "New before loading!";
                }
                MethodEntry* new_method = new_cl->vtp->GetVirtualEntry(nat);
                StackFrame* new_frame = set_new_stack_frame(new_method, frame);
                for (int j = 0; j < param_count + 1; j ++){
#ifdef DEBUG
                    cout << "Param " << j << " : " << thread_stack[frame->stack_top-param_count+j] << endl;
#endif
                    thread_stack[new_frame->local_variable_table+j] = thread_stack[frame->stack_top-param_count+j];
                }
                frame->stack_top -= (param_count + 1);
                interpret(new_method, new_cl, new_frame, thread_stack);
                break;
            }
            // invokespecial [indexbyte1 indexbyte2]
            case(0xb7):{
                // TO BE DONE: init
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                frame->stack_top -= 1;
                break;
            }
            // invokestatic [indexbyte1 indexbyte2]
            case(0xb8):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp); // MethodInfo
                item->resolved(cl->pcp);
                string full_name = item->GetNameAndType();
                ulong pos = full_name.find('.');
                string class_name = full_name.substr(0, pos);
                cout << full_name[pos] << endl;
                cout << "Class name: " << class_name << endl;
                pClass* new_cl = ClassLoader::findLoadedClass(class_name);
                pClass* dau_cl = ClassLoader::findLoadedClass("daughter");
                cout << "Father Add: " << (long)new_cl << " Daughter Add: " << (long)dau_cl << endl;
                cout << "New cl: " << (long)new_cl << " Old cl: " << (long)cl << endl;
                string nat = full_name.substr(pos+1, full_name.length() - pos + 1);
                cout << "Nat: " << nat << endl;
                ulong type_pos = nat.find(':');
                string type = nat.substr(type_pos+1, nat.length() - type_pos);
                int param_count = parse_param(type);
                //StaticMethodTable* smtp = cl->smtp;
                //MethodEntry* new_method = smtp->GetStaticMethodEntry(nat);
                MethodEntry* new_method = new_cl->GetStaticMethod(nat);
                cout << new_method->GetMethodResPos() << endl;
                //MethodEntry* father_method =
                //cout << "Address 1:" << (long)new_method << "  Address 2: " << (long)another_method << endl;
                StackFrame* new_frame = set_new_stack_frame(new_method, frame);
                for (int j = 0; j < param_count; j ++){
                    cout << "Params: " << thread_stack[frame->stack_top+1-param_count+j] << endl;
                    thread_stack[new_frame->local_variable_table+j] = thread_stack[frame->stack_top+1-param_count+j];
                }
#ifdef DEBUG
                cout << "Here!" << endl;
#endif
                frame->stack_top -= param_count;
                interpret(new_method, new_cl, new_frame, thread_stack);
                break;
            }
        }
    }
}

int main(int argc, char **argv){
    char *path = argv[1];
    int heap_size = atoi(argv[2]);
    heap = new Heap(heap_size);
    InitObjectPool(ObjectPool);
    ulong* thread_stack = new ulong[MAX_THREAD_STACK];
    memset(thread_stack, 0, MAX_THREAD_STACK);
    if (path == NULL){
        printf("ERROR: No file given!\n");
        exit(0);
    }
    int temp_char = 0;
    string test = string(path);
    for (int k = test.length()-1; k>=0; k--)
        if (test[k] == '/') {
            temp_char = k;
            break;
        }
    cout << test.substr(0, temp_char+1) << endl;
    ClassLoader::SetPath(test.substr(0, temp_char+1));
#ifdef DEBUG
    cout << "==================" << endl;
#endif
    ClassLoader::LoadClass(argv[1]);
#ifdef DEBUG
    cout << "==================" << endl;
#endif
    string tmp = string(path).substr(temp_char+1, 999);
    cout  << tmp.substr(0, tmp.length()-6) << endl;
    pClass* main_class = ClassLoader::findLoadedClass(tmp.substr(0, tmp.length()-6));
    MethodEntry* main_method;
    for(int i = 0; i < main_class->method_count; i ++){
        if (main_class->arrMethod[i]->GetName() == "main"){
            main_method = main_class->arrMethod[i];
        }
    }
    PureCode *main_code = GenCodePool.Access(main_method->GetMethodResPos());
    ushort max_locals = main_code->GetMaxLocals();
    ushort max_stack = main_code->GetMaxStack();
    StackFrame* initial_stack_frame = new StackFrame(max_locals, max_stack, NULL);
    interpret(main_method, main_class, initial_stack_frame, thread_stack);
    return 0;
}

