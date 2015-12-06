ENTRY(main)

MEMORY
{
	KERNEL (rwx) :
		ORIGIN = 10M,
		LENGTH = 20M
}

SECTIONS
{
	.text : {
		*(.text .rodata)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
}
