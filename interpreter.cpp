#include "interpreter.h"
#include "ClassLoader.h"

StackFrame::StackFrame(uint max_locals, uint max_stack, StackFrame* call){
    start_pos = call->end_pos;
    return_address = start_pos + 1;
    dynamic_linking = start_pos + 2;
    local_variable_table = start_pos + 3;
    operand_stack_start = start_pos + 3 + max_locals;
    stack_top = operand_stack_start;
    end_pos = operand_stack_start + max_stack;
    frame_length = end_pos - start_pos;
    caller = call;
}

StackFrame* set_new_stack_frame(MethodEntry* method, StackFrame* top){
    PureCode* code = GenCodePool.Access(method->GetMethodResPos());
    ushort max_locals = code->GetMaxLocals();
    ushort max_stack = code->GetMaxStack();
    StackFrame* new_frame = new StackFrame(max_locals, max_stack, top);
    return new_frame;
}

void interpret(MethodEntry* method, pClass* cl, StackFrame* frame, ulong* thread_stack){
    PureCode* code = GenCodePool.Access(method->GetMethodResPos());
    uint code_length = code->GetCodeLength();
    int count = 0;
    uchar tmp;
    while(count < code_length){
        uchar bytecode = code->GetNextCode();
        switch(bytecode){
            // dup
            case (0x59):{
                thread_stack[frame->stack_top] = thread_stack[frame->stack_top-1];
                frame->stack_top++;
                break;
            }
            // dup2
            case (0x5c):{
                thread_stack[frame->stack_top+1] = thread_stack[frame->stack_top-1];
                thread_stack[frame->stack_top] = thread_stack[frame->stack_top-2];
                frame->stack_top+=2;
                break;
            }
            // aload_0
            case (0x2a):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table];
                frame->stack_top++;
                break;
            }
            // aload_1
            case (0x2b):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+1];
                frame->stack_top++;
                break;
            }
            // aload_2
            case (0x2c):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+2];
                frame->stack_top++;
                break;
            }
            // aload_3
            case (0x2d):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+3];
                frame->stack_top++;
                break;
            }
            // iload_0
            case (0x1a):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table];
                frame->stack_top++;
                break;
            }
            // iload_1
            case (0x1b):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+1];
                frame->stack_top++;
                break;
            }
            // iload_2
            case (0x1c):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+2];
                frame->stack_top++;
                break;
            }
            // iload_3
            case (0x1d):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+3];
                frame->stack_top++;
                break;
            }
            // lload_0
            case (0x1e):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table];
                frame->stack_top++;
                break;
            }
            // lload_1
            case (0x1f):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+1];
                frame->stack_top++;
                break;
            }
            // lload_2
            case (0x20):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+2];
                frame->stack_top++;
                break;
            }
            // lload_3
            case (0x21):{
                thread_stack[frame->stack_top] = thread_stack[frame->local_variable_table+3];
                frame->stack_top++;
                break;
            }
            // astore_0
            case (0x4b):{
                thread_stack[frame->local_variable_table] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // astore_1
            case (0x4c):{
                thread_stack[frame->local_variable_table+1] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // astore_2
            case (0x4d):{
                thread_stack[frame->local_variable_table+2] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
            }
            // astore_3
            case (0x4e):{
                thread_stack[frame->local_variable_table+3] = thread_stack[frame->stack_top];
                frame->stack_top--;
                break;
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
            // new [0 index]
            case(0xbb):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp);
                ushort class_name_index = *(ushort*)item->GetValue();
                string class_name = *(string*)cl->GetConstantPoolItem(class_name_index)->GetValue();
                class_attribute* new_cl = new class_attribute((char*)class_name.c_str());
                pClass* new_class = ClassLoader::findLoadedClass(class_name);
                if (!new_class){
                    if(!ClassLoader::LoadClass(new_cl))
                        throw "Load Class Failed!";
                    new_class = ClassLoader::findLoadedClass(class_name);
                }
                // 未完待续
                StackFrame* new_frame = set_new_stack_frame(new_method, frame);
                interpret(new_method, new_cl, new_frame, thread_stack);
                break;
            }
            // invokevirtual [indexbyte1 indexbyte2]
            case(0xb6):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp); // MethodInfo
                item->resolved(cl->pcp);
                string static_name = item->GetNameAndType();
                // 未完待续
                pClass* new_cl;
                MethodEntry* new_method;
                StackFrame* new_frame = set_new_stack_frame(new_method, frame);
                interpret(new_method, new_cl, new_frame, thread_stack);
                break;
            }
            // invokestatic [indexbyte1 indexbyte2]
            case(0xb8):{
                tmp = code->GetNextCode();
                tmp = code->GetNextCode();
                ConstantPoolMetaType* item = cl->GetConstantPoolItem(tmp); // MethodInfo
                item->resolved(cl->pcp);
                string static_name = item->GetNameAndType();
                // 未完待续
                break;
            }
        }
    }
}

int main(int argc, char **argv){
    char *path = argv[1];
    ulong* thread_stack = new ulong[MAX_THREAD_STACK];
    if (path == NULL){
        printf("ERROR: No file given!\n");
        exit(0);
    }
    class_attribute* cl = new class_attribute(path);
    if (ClassLoader::LoadClass(cl) == 0){
        throw "Load Class Failed!";
    }
    pClass* main_class = ClassLoader::findLoadedClass(path);
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
    return 0;
}

