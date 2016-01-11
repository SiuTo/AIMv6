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
	uart_spin_puts("Pending...\r\n");
	remove_low_mapping();
	uart_spin_puts("Pending...\r\n");
	init_freelist();
	uart_spin_puts("Pending...\r\n");
	sched_init();
	uart_spin_puts("Pending...\r\n");

	slab_cache_t pcb_pool;
	init_slab_cache(&pcb_pool, sizeof(pcb_t), 10, 2, 1);
	pcb_t* pcb = (pcb_t *)alloc_obj(&pcb_pool);
	pcb->td.pid = 123;
	sched_new(pcb);
	sched_mature(pcb);
	sched_allow(pcb);
	sched_finish(pcb);
	pcb_t *find = sched_get_bypid(3);
	if (find == NULL) uart_spin_puts("NULL\r\n");
	else puthex(find->td.pid);
	find = sched_get_bypid(123);
	if (find == NULL) uart_spin_puts("NULL\r\n");
	else puthex(find->td.pid);
	
	uart_spin_puts("Pending...\r\n");
	while (1);
	return 0;
}

