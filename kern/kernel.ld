ENTRY(main)

MEMORY
{
	KERNEL (rwx) :
		ORIGIN = 10M,
		LENGTH = 200M
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
