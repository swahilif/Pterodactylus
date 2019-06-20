rm *.o
g++ ObjectHeader.cpp -o ObjectHeader.o -c -std=c++11
g++ Object.cpp -o Object.o -c -std=c++11
g++ javap.cpp -o javap.o -c -std=c++11
g++ Field.cpp -o Field.o -c -std=c++11
g++ StaticMethodTable.cpp -o StaticMethodTable.o -c -std=c++11
g++ MethodPool.cpp -o MethodPool.o -c -std=c++11
g++ ConstantPool.cpp -o ConstantPool.o -c -std=c++11
g++ VirtualTable.cpp -o VirtualTable.o -c -std=c++11
g++ interpreter.cpp -o interpreter.o -c -std=c++11
g++ ClassLoader.cpp -o ClassLoader.o -c -std=c++11
g++ ClassFile.cpp -o ClassFile.o -c -std=c++11
g++ GarbageCollect.cpp -o GarbageCollect.o -c -std=c++11
g++ HeapManager.cpp -o HeapManager.o -c -std=c++11
g++ *.o -o interpreter -std=c++11
