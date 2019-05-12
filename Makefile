javap.o : 
	g++ -c src/javap.cpp -o javap.o -std=c++11	
VirtualTable.o :
	g++ -c src/VirtualTable.cpp -o VirtualTable.o -std=c++11
clean :
	rm *.o
