#include "syscall.h"
// #include "iostream.h"

main()
{
	char* filename = "./file.txt";
	Create(filename);
	int id = Open(filename);
	
	Write(filename, 10, id);
	// Write(filename, 10, id+1);
	// std::cout << v << std::endl;
	Close(id);
	
	int id2 = Open(filename);
	char buffer[6];
	Read(buffer, 5, id2);
	PrintMsg(buffer);
	Close(id2);
}
