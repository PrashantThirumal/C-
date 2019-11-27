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
