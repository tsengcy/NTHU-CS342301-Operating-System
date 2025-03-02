# NTHU CS342301 Operating System
This repository contains the homework for the open course on Operating Systems provided by NTHU.([link](<https://ocw.nthu.edu.tw/ocw/index.php?page=course&cid=141&>))

## Install Nachos
1. Install Ubuntu14.04 32-bits ([link](https://releases.ubuntu.com/14.04/)) on VirtualBox.

2. Install C shell (csh) and g++
    ```bash
    sudo apt-get install csh
    sudo apt-get install g++
    ```
3. Download and compile Nachos 4.0 and mips-x86.linux-xgcc (Prof. Wang's website).
    ```bash
    # download 
    wget -d http://cc.ee.ntu.edu.tw/~farn/courses/OS/OS2015/projects/project.1/mips-x86.linux-xgcc.tar.gz
    wget -d http://cc.ee.ntu.edu.tw/~farn/courses/OS/OS2015/projects/project.1/nachos-4.0.tar.gz 
    # unzip file
    tar -zvxf nachos-4.0.tar.gz
    sudo mv mips-x86.linux-xgcc.tar.gz /
    cd /
    tar -zxvf /mips-x86.linux-xgcc.tar.gz
    # Compile
    cd ~/nachos-4.0/code
    make
    ```
4. test
    ```bash
    ./userprog/nachos -e ./test/test1

    # Total threads number is 1
    # Thread test/test1 is executing
    # Print integer:9
    # Print integer:8
    # Print integer:7
    # Print integer:6
    # Print integer:5
    # return value:0
    # No threads ready or runnable, and no pending interrupts.
    # Assuming the program completed.
    # Machine halting!
    ```

5. Install Error 

    if facing the compiling error 
    ```
    make[1]: /usr/local/nachos/decstation-ultrix/bin/gcc: Command not found
    ```
    Modifiy the Makefile file in ```test``` 
    ```bash
    # GCC = /usr/local/nachos/decstation-ultrix/bin
    GCC = /mips-x86.linux-xgcc/
    
    # CPP = /lib/cpp
    CPP = /mips-x86.linux-xgcc/cpp0

    # CFLAGS = -G 0 -c $(INCDIR)
    CFLAGS = -G 0 -c $(INCDIR) -B/mips-x86.linux-xgcc/
    ```
## Reference
- OS2020 Nachos Project ([link](https://github.com/pai4451/OS2020))
- [作業系統] Nachos 安裝 ([link](https://jeffprogrammer.wordpress.com/2016/10/31/%E4%BD%9C%E6%A5%AD%E7%B3%BB%E7%B5%B1-nachos-%E7%B0%A1%E4%BB%8B/))
