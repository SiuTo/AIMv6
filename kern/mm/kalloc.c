#include <kernel.h>
#include <drivers/serial/uart.h>

struct run
{
	int size;
	struct run *next;
} *free_list;

void init_freelist(void *addr, int num)
{
	free_list = (struct run *)addr;
	free_list->size = num;
	free_list->next = 0;
}

char* alloc_pages(int num)
{
	struct run *last;
	for (struct run *p=free_list; p; p=p->next)
	{
		if (p->size>num)
		{
			p->size -= num;
			return (char *)p;
		}
		else if (p->size==num)
		{
			if (p==free_list) free_list = p->next; else
			{
				last->next = p->next;
			}
			return (char *)p;
		}
		last = p;
	}

	uart_spin_puts("Error: alloc_pages\r\n");
	while (1);
}

// TODO: merge
void free_pages(char *addr, int num)
{
	struct run *r = (struct run *)addr;
	r->size = num;
	
	if ((char *)free_list<addr)
	{
		r->next = free_list;
		free_list = r;
		return;
	}

	struct run *p = free_list;
	while (p->next && (char *)p->next > addr) p = p->next;
	r->next = p->next;
	p->next = r;

}

