#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Flags for bitwise operations
#define FLAG_0        (1 << 0) // 1
#define FLAG_1        (1 << 1) // 2
#define FLAG_2        (1 << 2) // 4
#define FLAG_3        (1 << 3) // 8

// Constant variable for testing
const int const_int = 5;

// Global Variable
int global_int = 5;

// Functions to parse vm_flags
void parse(long num) {
  int exists = 0;

  if (num & FLAG_0) {
    printf("INTIALIZED\n");
    exists += 1;
  }

  printf("\nPrinting Permissions of VMA");

  if (num & FLAG_1) {
    printf("\nREAD ACCESS\n");
    exists += 1;
  }
  if (num & FLAG_2) {
    printf("\nWRITE ACCESS\n");
    exists += 1;
  }
  if (num & FLAG_3) {
    printf("\nEXEC ACCESS\n");
    exists += 1;
  }

  if (exists == 0) {
    printf("\nNOT FOUND\n");
  }
  
}


int main( int argc, char* argv[] ) {
  
  int local_int = 7;
  unsigned long countValid = 0;
  unsigned long countInvalid = 0;
  
	unsigned long PAGE_SIZE = 0;
	unsigned long TASK_SIZE = 0;

	PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
  
	// For 32 bit system
	if ( sizeof(void*) == sizeof(int) ) {
		TASK_SIZE = 0xc0000000UL;
	}
	// For 64 bit
	else {
		TASK_SIZE = (1UL << 47) - PAGE_SIZE;
	}
  
  
  printf("\nGLOBAL VARIABLE\n");
  long global_parse = syscall(336, &global_int);
  parse(global_parse);
  printf("========================================================\n");

  printf("\nLOCAL VARIABLE\n");
  long local_parse = syscall(336, &local_int);
  parse(local_parse);
  printf("========================================================\n");

  printf("\nMALLOCED BUFFER\n");
  char *buf = (char*)malloc(4);
  long malloc_buf = syscall(336, &buf);
  parse(malloc_buf);
  printf("========================================================\n");


  printf("\nFUNCTION\n");
  long func = syscall(336, &parse);
  parse(func);
  printf("========================================================\n");


  printf("\nNULL ADDRESS\n");
  int *null_ptr = NULL;
  long null_int = syscall(336, &null_ptr);
  parse(null_int);
  printf("========================================================\n");

  printf("\nCONSTANT VARIABLE\n");
  long cons = syscall(336, &const_int);
  parse(cons);
  printf("========================================================\n");


  //printf("STARTING FOR LOOP NOW\n");
  char *p = 0;
  long bits = 0;
  for( ; (unsigned long)p < TASK_SIZE; p += PAGE_SIZE*1024) {  
    bits = syscall(336, p);
    if ((bits & FLAG_0) == 1 ) {
      //parse(bits);
      countValid++;
      //break;
    }
    countInvalid++; 
  } 

  printf("%lu out of %lu addresses are allocated\n", countValid, countValid + countInvalid);
    
	return 0;
	
}
