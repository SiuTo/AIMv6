#include <drivers/serial/uart.h>
#include <kernel.h>

void mmu_enable()
{
	uart_spin_puts("Enable MMU\r\n");

	// build the page table
	for (u32 i=0; i<4096; ++i)
	{
		u32 *entry_addr = (u32 *)(KPGDIR_BASE + (i<<2));
		*entry_addr = (i<<20) | 0x5E2;
	}
	for (u32 i=0; i<512; ++i)
	{
		u32 *entry_addr = (u32 *)(KPGDIR_BASE + (((KERN_BASE>>20) + i)<<2));
		*entry_addr = (i<<20) | 0x5E2;
	}

	// set TTB register
	asm volatile (
		"mov r0, %0\n\t"
		"mcr p15, 0, r0, c2, c0, 0\n\t"
		:
		:"r"(KPGDIR_BASE)
		:"r0"
	);

	// set domain access control register
	asm (
		"ldr r0, =0x55555555\n\t"
		"mcr p15, 0, r0, c3, c0, 0\n\t"
	);

	// enable MMU
	asm (
		"mrc p15, 0, r0, c1, c0, 0\n\t"
		"orr r0, r0, #0x1\n\t"
		"mcr p15, 0, r0, c1, c0, 0\n\t"
	);

	// invalidate TLB
	asm (
		"mov r0, #0\n\t"
		"mcr p15, 0, r0, c8, c5, 0\n\t"
		"mcr p15, 0, r0, c8, c6, 0\n\t"
		"mcr p15, 0, r0, c8, c7, 0\n\t"
	);

	// jump to high memory
	asm volatile(
		"mov r0, %0\n\t"
		"add pc, pc, r0\n\t"
		"add sp, sp, r0\n\t"
		"add fp, fp, r0\n\t"
		:
		:"r"(KERN_BASE)
		:"r0"
	);

	uart_spin_puts("MMU started!\r\n");
}

void remove_low_mapping()
{
	for (u32 i=0; i<512; ++i)
	{
		u32 *entry_addr = (u32 *)p2v(KPGDIR_BASE + (i<<2));
		*entry_addr = 0;
	}
}

