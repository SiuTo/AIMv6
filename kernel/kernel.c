#define PRELOAD_VECTOR_BASE	0x1ff00000

void (*uart_spin_puts)(const char *) = (void *)(PRELOAD_VECTOR_BASE + 0xC);

int main(void)
{
	uart_spin_puts("Kernel started!\r\n");
	return 0;
}

