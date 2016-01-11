/*
 * Copyright (C) 2015 David Gao <davidgao1001@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#define ELF32

#include <config.h>
#include <elf.h>

#define PRELOAD_VECTOR_BASE	0x1ff00000


void (*uart_init)(void) = (void *)(PRELOAD_VECTOR_BASE + 0x4);
void (*uart_enable)(void) = (void *)(PRELOAD_VECTOR_BASE + 0x8);
void (*uart_spin_puts)(const char *) = (void *)(PRELOAD_VECTOR_BASE + 0xC);
int (*sd_dma_spin_read)(u32, u16, u32) = (void *)(PRELOAD_VECTOR_BASE + 0x10);
void (*puthex)(u32) = (void *)(PRELOAD_VECTOR_BASE + 0x14);

void mbr_bootmain(void)
{
	uart_spin_puts("Running mbr...\r\n");
	volatile u8 *mbr = (void *)0x100000;
	u32 sd_base_addr = (u32)mbr[470] + ((u32)mbr[471]<<8) + ((u32)mbr[472]<<16) + ((u32)mbr[473]<<24);

	u32 mem_base_addr = 0x100200;
	sd_dma_spin_read(mem_base_addr, 1, sd_base_addr);
	elfhdr_t *elf_header = (void *)mem_base_addr;
	sd_dma_spin_read(mem_base_addr+0x200, byteToSector(elf_header->e_phentsize*elf_header->e_phnum), sd_base_addr);
	elf_phdr_t *elf_pheader = (void *)(mem_base_addr+0x200+elf_header->e_phoff);

	for (int i=0; i<elf_header->e_phnum; ++i)
	{
		if (elf_pheader[i].p_type==PT_LOAD)
		{
			puthex(elf_pheader[i].p_vaddr);
			uart_spin_puts("\r\n");
			puthex(elf_pheader[i].p_memsz);
			uart_spin_puts("\r\n");
			puthex(elf_pheader[i].p_offset);
			uart_spin_puts("\r\n");
			sd_dma_spin_read(elf_pheader[i].p_vaddr-(elf_pheader[i].p_offset&511), byteToSector(elf_pheader[i].p_memsz), sd_base_addr+(elf_pheader[i].p_offset>>9));
			uart_spin_puts("finish read...\r\n");
		}
	}
	
	int (*kernel_main)(void) = (void *)elf_header->e_entry;
	kernel_main();
}

int byteToSector(int x)
{
	return ((x-1)>>9)+1;
}

