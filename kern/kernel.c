#include <drivers/serial/uart.h>
#include <kern/kernel.h>
#include <kern/mm/kalloc.h>
#include <kern/mm/mmu.h>
#include <kern/mm/slab.h>
#include <kern/sched/pcb.h>
#include <kern/sched/policy_rr.h>

int main(void)
{
	uart_spin_puts("Kernel started!\r\n");

	mmu_enable();
	remove_low_mapping();
	init_freelist();
	sched_init();
	
	uart_spin_puts("Pending...\r\n");
	while (1);
	return 0;
}

