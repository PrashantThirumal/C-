#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/syscalls.h>

// This Wrapper is necessary
SYSCALL_DEFINE1(readAddr, void*, p) {
  long intital = 1;
	
  struct task_struct *task = current;
  struct mm_struct *mm;
  struct vm_area_struct *mmap;

  task = get_current();	
  mm = task -> mm;

  // Use find vma instead of cycling through the vma linked list
  mmap = find_vma(mm, (unsigned long)p);
  
  if (mmap == NULL) {
    printk("\nFind VMA returned NULL\n");
    return 0;
  }
	
  if( p >= mmap -> vm_end ) {
    printk ("\nPointer at end of VMA\n");
    return 0;
  }
  
  printk("\nStarts at 0x%lx, Ends at 0x%lx\n", mmap -> vm_start, mmap -> vm_end);
  return ( ( (mmap -> vm_flags) << 1 ) | intital);

	
}
