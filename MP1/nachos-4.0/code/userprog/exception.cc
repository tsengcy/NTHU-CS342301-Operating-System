// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

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
		case SC_Halt:
		    DEBUG(dbgAddr, "Shutdown, initiated by user program.\n");
   		    kernel->interrupt->Halt();
		    break;
		case SC_PrintInt:
			val=kernel->machine->ReadRegister(4);
			// cout << "Print integer:" <<val << endl;
			cout << val << endl;
			return;
/*		case SC_Exec:
			DEBUG(dbgAddr, "Exec\n");
			val = kernel->machine->ReadRegister(4);
			kernel->StringCopy(tmpStr, retVal, 1024);
			cout << "Exec: " << val << endl;
			val = kernel->Exec(val);
			kernel->machine->WriteRegister(2, val);
			return;
*/		case SC_Exit:
			DEBUG(dbgAddr, "Program exit\n");
			val=kernel->machine->ReadRegister(4);
			cout << "return value:" << val << endl;
			kernel->currentThread->Finish();
			break;
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

		case SC_PrintMsg:
			val = kernel->machine->ReadRegister(4);
			msg = &(kernel->machine->mainMemory[val]);
			std::cout << msg << std::endl;

			
			
			
		case SC_DoubleInt:
			val = kernel->machine->ReadRegister(4);
			// cout << "value: " << val * 2 << endl;
			kernel->machine->WriteRegister(2, val*2);
			return;
		default:
		    cerr << "Unexpected system call " << type << "\n";
 		    break;
	    }
	    break;
	default:
	    cerr << "Unexpected user mode exception" << which << "\n";
	    break;
    }
    ASSERTNOTREACHED();
}
