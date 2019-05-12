#Compilation Macro
CC = g++
CFLAGS = -Wall
LDFLAGS = -lm

interpreter:interpreter.o VirtualTable.o Field.o javap.o
	${CC} 

VirtualTable.o :
	${CC} -c src/VirtualTable.cpp -o VirtualTable.o -std=c++11
javap.o : 
	${CC} -c src/javap.cpp -o javap.o -std=c++11	
Field.o :
	${CC} -c src/Field.cpp -o Field.o -std=c++11
clean :
	rm *.o
