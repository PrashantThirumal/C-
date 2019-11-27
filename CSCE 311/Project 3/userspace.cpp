#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
  // Hello World will print to kernel log
  long int amma = syscall(335);
  cout << "System call sys_hello returned " << amma << endl;
  return 0;
}
