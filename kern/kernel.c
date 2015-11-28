#include <drivers/serial/uart.h>
#include <kernel.h>

int main(void)
{
	uart_spin_puts("Kernel started!\r\n");
	mmu_enable();
	return 0;
}

