# MP1-System Call

1. Implement a console I/O system call

    This code have already done in the Nachos file.

    userprog/exception.cc
    ```cpp
    void ExceptionHandler(ExceptionType which){
        int type = kernel->machine->ReadRegister(2);
        switch (which) {
	    case SyscallException:
            switch(type){
                case SC_PrintInt:
			    val=kernel->machine->ReadRegister(4);
			    cout << "Print integer:" <<val << endl;
			    return;
            }
        }
    }
    ```
    According the the commit in the code. The system call code and the parameter can be read from register 2 and register 4. Then, print it out with cout.
    
    test/start.s
    ```asm
        .globl  PrintInt
	    .ent    PrintInt
    PrintInt:
	    addiu   $2,$0,SC_PrintInt
	    syscall
	    j       $31
	    .end    PrintInt
    ```

    userprog/syscall.h
    ```cpp
    #define SC_PrintInt	11
    void PrintInt(int number);
    ```
    define the system call code and the function for the system to used in syscall file.

2. Implement four file I/O system call.

	filesys/filesys.h
	```cpp
	class FileSystem {
	public:
    	FileSystem(bool format=true) {bMap = new BitMap(32);}

    	~FileSystem(){delete bMap;}

    	bool Create(char *name, int initialSize) { 
			int fileDescriptor = OpenForWrite(name);

			if (fileDescriptor == -1) return FALSE;
			Close(fileDescriptor); 
			return TRUE; 
		}

    	OpenFile* Open(char *name) {
			int fileDescriptor = OpenForReadWrite(name, FALSE);

			if (fileDescriptor == -1) return NULL;
			bMap->Mark(fileDescriptor);
			return new OpenFile(fileDescriptor);
    	}

    	bool Remove(char *name) { return Unlink(name) == 0; }
	
    	bool isFile(int sector){
			if(bMap->Test(sector)) return true;
			else return false;
    	}

    	bool close(int id){
			if(Close(id) == 1)
			{
				bMap->Clear(id);
				return true;
			}
			else return false;
    	}

	private:
    	BitMap* bMap;
	};
	```
	FileSystem::isFile(int) is used to test whether there is a file in the file system and FileSystem::close(int) is used to close the file. the BitMap is used to memory the file id that have been create. 

    userprog/exception.cc
    ```cpp
    void
	ExceptionHandler(ExceptionType which)
	{
		int		type = kernel->machine->ReadRegister(2);
		int		val;
		char*		msg;
		int 		id;
		int		size;
		OpenFile*	OFile;
		// char*	ch;

	    switch (which) {
		case SyscallException:
		    switch(type) {
			case SC_Create:
				val = kernel->machine->ReadRegister(4);
				msg = &(kernel->machine->mainMemory[val]);
				cout << "filename: " << msg << endl;
				val = kernel->fileSystem->Create(msg, 1024);

				kernel->machine->WriteRegister(2, val);
				std::cout << "create file: " << val << std::endl;
				return;
			case SC_Open:
				val = kernel->machine->ReadRegister(4);
				msg = &(kernel->machine->mainMemory[val]);
				cout << "filename: " << msg << endl;
				OFile = kernel->fileSystem->Open(msg);

				if(OFile == NULL){ /*fail*/
					kernel->machine->WriteRegister(2, -1);
					std::cout << "fail to open" << std::endl;
				}else{
					kernel->machine->WriteRegister(2, OFile->getFileid());
					std::cout << "Open: " << OFile->getFileid() << std::endl;
				}

				return;
			case SC_Write:
				val = kernel->machine->ReadRegister(4);
				msg = &(kernel->machine->mainMemory[val]);

				size = kernel->machine->ReadRegister(5);
				id = kernel->machine->ReadRegister(6);

				if(!kernel->fileSystem->isFile(id)){
					kernel->machine->WriteRegister(2, -1);
					cout << "attempt writing to an invalid id." << endl;
					return;
				}

				OFile = new OpenFile(id);
				OFile->Write(msg, size);
				kernel->machine->WriteRegister(2, OFile->getFileid());
				return;
			case SC_Read:
				val = kernel->machine->ReadRegister(4);
				msg = &(kernel->machine->mainMemory[val]);
				size = kernel->machine->ReadRegister(5);
				id = kernel->machine->ReadRegister(6);

				if(!kernel->fileSystem->isFile(id)){
					kernel->machine->WriteRegister(2, -1);
					cout << "attempt reading to an invalid id." << endl;
					return;
				}

				OFile = new OpenFile(id);
				val = OFile->Read(msg, size);
				kernel->machine->WriteRegister(2, val);
				return;
			case SC_Close:
				id = kernel->machine->ReadRegister(4);
				if(kernel->fileSystem->close(id)){
					kernel->machine->WriteRegister(2, 1);
					std::cout << "close" << std::endl;
				}else{
					kernel->machine->WriteRegister(2, 0);
					std::cout << "fail to close" << std::endl;
				}
				return;
		    }
	    }
	}
    ```
    use the filesystem to implenment Open, Write, Read, Close four function.

    