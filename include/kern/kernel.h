#ifndef _KERNEL_H
#define _KERNEL_H

#include <sys/types.h>

#define KERN_BASE 0x80000000
#define KPGDIR_BASE 0x200000
#define PHY_SIZE 0x20000000
#define PHY_BEGIN 0x2800000
#define PHY_END 0x1ff00000
#define PAGE_SIZE 0x1000
#define PAGE_SIZE_SHIFT 12

static inline void *p2v(u32 p) { return (void *)(p+KERN_BASE); }

#endif

