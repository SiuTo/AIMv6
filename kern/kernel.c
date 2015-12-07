#include <drivers/serial/uart.h>
#include <kernel.h>
#include <kalloc.h>
#include <mmu.h>

int main(void)
{
	uart_spin_puts("Kernel started!\r\n");

	mmu_enable();
	remove_low_mapping();
	init_freelist();
	
	uart_spin_puts("Pending...\r\n");
	while (1);
	return 0;
}

