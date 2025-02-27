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
   
    userprog/exception.cc
    ```cpp
    void ExceptionHandler(ExceptionType which){
        int	type = kernel->machine->ReadRegister(2);
        int	val;
        char* msg;
        int id;
	    int	size;
	    OpenFile*	OFile;

        switch (which) {
	    case SyscallException:
            switch(type) {
                case SC_Create:
			        val = kernel->machine->ReadRegister(4);
			        msg = &(kernel->machine->mainMemory[val]);
			        val = kernel->fileSystem->Create(msg, 1024);

			        kernel->machine->WriteRegister(2, val);
			        return;
		        case SC_Open:
			        val = kernel->machine->ReadRegister(4);
			        msg = &(kernel->machine->mainMemory[val]);
			        OFile = kernel->fileSystem->Open(msg);
			
			        if(OFile == NULL){ /*fail*/
				        kernel->machine->WriteRegister(2, -1);
			        }else{
				        kernel->machine->WriteRegister(2, OFile->getFileid());
			        }
			        return;
		        case SC_Write:
			        val = kernel->machine->ReadRegister(4);
			        msg = &(kernel->machine->mainMemory[val]);
			
			        size = kernel->machine->ReadRegister(5);
			        id = kernel->machine->ReadRegister(6);
			
			        OFile = new OpenFile(id);
			        OFile->Write(msg, size);

			        return;
		        case SC_Read:
			        val = kernel->machine->ReadRegister(4);
			        msg = &(kernel->machine->mainMemory[val]);
			        size = kernel->machine->ReadRegister(5);
			        id = kernel->machine->ReadRegister(6);

			        OFile = new OpenFile(id);
			        val = OFile->Read(msg, size);
			        kernel->machine->WriteRegister(2, val);
			        return;
		        case SC_Close:
		        	id = kernel->machine->ReadRegister(4);
		        	Close(id);
		        	kernel->machine->WriteRegister(2, 1);
		        	std::cout << "close" << std::endl;
		        	return;
            }
        }
    }
    ```
    Refer to the code of filesys/openfile.cc, the function OpenFile::Write(char* int0, int numBytes) call WriteAt to fulfill the file operations which is written in lib/syscall.h.

    