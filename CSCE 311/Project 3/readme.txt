In this project, I wrote two system calls in the kernel that can be called by the user.
The first syscall is syscall number 335. This system call simply prints "Hello World" to the kernel log
The second syscall is syscall number 336. This system call returns the permissions(VM_FLAGS) of the VMA of a pointer. 
It also prints the VMA address to the Kernel log


There are two files that display the functionality of the two system calls.
1. Hello world syscall:
Compile: g++ -o usrspc userspace.cpp
Run: ./usrspc

2. Read Address system call
Compile: gcc -o test readAddr.c
Run: ./test


Additional Information:
To Access kernel log: dmesg
Neccessary include: #include <unistd.h>

Here are the steps I took
Steps:
1.) unpack linux-5.3.12
2.) mkdir hello, mkdir readAddr
3.) gedit Makefile [obj-y := hello.o] / [obj-y := readAddr.o]
4.) cd /include/linux/sycalls.h
5.) asmlinkage long sys_hello(void);
    asmlinkage long sys_readAddr(void *p);
6.)cd /include/uapi/asm-generic/unistd.h
7.)line 854: #define __NR_readAddr 336
   line 855: __SYSCALL(__NR_readAddr, sys_readAddr)
8.)cd /kernel/sys_ni.c
9.)line 453 COND_SYSCALL(readAddr);
10.)cd /arch/x86/entry/syscalls/syscall_64.tbl
11.)line 346: 335  64  hello   sys_hello
    line 347: 336  64  readAddr   __x64_sys_readAddr
12.)gedit Makefile(in default kernel)
13.)line 1011: block/ hello/ readAddr/


One thing I do not understand is that when I do the same steps I did for the hello world syscall the readAddr syscall will not work.
The error I got was that the readAddr syscall cannot be found. Hence steps 6 - 8 is unique to just the readAddr syscall
