# MP2-Multi-Programming

## Problem
The NachOS doesn’t support multi-programming now, you have to modify the code to make NachOS support it. By memory management, Nachos will
arrange memory to reach multi-programming.

Original Result:
```
Total threads number is 2
Thread test/test1 is executing.
Thread test/test2 is executing.
9
8
7
20
21
22
23
24
6
7
8
9
10
12
13
14
15
16
16
17
18
19
20
17
18
19
20
21
21
23
24
25
return value:0
26
return value:0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 800, idle 67, system 120, user 613
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```

Because two program share the same physical memory, so the value can be access by either test1 or test2 program. This cause the unexcept output for each program.
Therefore there need a method to allocate the physical memory for each program.

## Solution
For each Thread, it do not allocate all pages (NumPhysPages) for it. In ```bool AddrSpace::Load(char*)``` will calculate the number of page that needed for this thread. Therefore, allocate the physical memory at this time. also inorder not to reused the memory. A static boolean array ```PhysPagesUsed``` is defined to check whether the page is used.

userprog/addrspace.h
```cpp
class AddrSpace {
    ...
    static bool PhysPagesUsed[NumPhysPages];  // define the array prevent from reuse the page.
    ...
}
```
userprog/addspace.cpp
```cpp
bool AddrSpace::PhysPagesUsed[NumPhysPages] = {0};  // set the inital value to false

AddrSpace::AddrSpace(){
/*
    pageTable = new TranslationEntry[NumPhysPages];
    for (unsigned int i = 0; i < NumPhysPages; i++) {
	pageTable[i].virtualPage = i;	// for now, virt page # = phys page #
	pageTable[i].physicalPage = i;
	pageTable[i].valid = TRUE;
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE;  
    }
*/ // remove the original page table definition.
...
}

AddrSpace::~AddrSpace()
{
   for(unsigned int i=0; i<numPages; i++)
   {
	PhysPagesUsed[pageTable[i].physicalPage] = false;
   }
   // reset the value in the array.
   delete pageTable;
}

bool 
AddrSpace::Load(char *fileName) 
{
    ... 
    numPages = divRoundUp(size, PageSize);
// inintalize the page table based on the memory needed in this process.
    pageTable = new TranslationEntry[numPages];
    for (unsigned int i = 0, j = 0; i < numPages; i++) {
	pageTable[i].virtualPage = i;
	while(j < NumPhysPages && PhysPagesUsed[j] == true) j++;
	ASSERT(j != NumPhysPages);
	pageTable[i].physicalPage = j;
	PhysPagesUsed[j] = true;
	pageTable[i].valid = TRUE;
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE;  
    }
    size = numPages * PageSize;
    ...

	if (noffH.code.size > 0) {
        DEBUG(dbgAddr, "Initializing code segment.");
	    DEBUG(dbgAddr, noffH.code.virtualAddr << ", " << noffH.code.size);
// modify the address to the mainMemory, 
//      pageTable[noffH.code.virtualAddr/PageSize].physicalPage * PageSize + noffH.code.virtualAddr % PageSize
// is the address translation scheme for the page table.
        executable->ReadAt(
		&(kernel->machine->mainMemory[pageTable[noffH.code.virtualAddr/PageSize].physicalPage * PageSize + 
							noffH.code.virtualAddr % PageSize]), 
			noffH.code.size, noffH.code.inFileAddr);
    }
	if (noffH.initData.size > 0) {
        DEBUG(dbgAddr, "Initializing data segment.");
	    DEBUG(dbgAddr, noffH.initData.virtualAddr << ", " << noffH.initData.size);
// modify the address to the mainMemory 
        executable->ReadAt(
		&(kernel->machine->mainMemory[pageTable[noffH.initData.virtualAddr/PageSize].physicalPage * PageSize + 
							noffH.initData.virtualAddr % PageSize]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }
    ...
}

```