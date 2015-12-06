#ifndef _KERNEL_H
#define _KERNEL_H

#define KERN_BASE 0x80000000
#define KPGDIR_BASE 0x200000
#define PHY_SIZE 0x20000000
#define PAGE_SIZE 0x1000

void mmu_enable();

#endif

