#ifndef _KERNEL_H
#define _KERNEL_H

#define KERN_BASE 0x80000000
#define KPGDIR_BASE 0x200000

void mmu_enable();

#endif

