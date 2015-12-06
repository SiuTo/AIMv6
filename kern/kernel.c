#include <drivers/serial/uart.h>
#include <kernel.h>

int main(void)
{
	uart_spin_puts("Kernel started!\r\n");

	// kernel stack
	asm (
		"ldr sp, =0x2800000\n\t"
		"movs fp, sp\n\t"
	);

	mmu_enable();
	while (1);
	return 0;
}

