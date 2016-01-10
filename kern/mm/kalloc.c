#include <kern/kernel.h>
#include <config.h>
#include <drivers/serial/uart.h>

typedef struct run
{
	int size;
	struct run *next;
} run_t;

run_t *free_list;

void init_freelist()
{
	run_t *head = (run_t *)p2v(PHY_BEGIN);
	run_t *mid = (run_t *)p2v(PHY_BEGIN+(PAGE_SIZE<<1));
	run_t *tail = (run_t *)p2v(PHY_END-PAGE_SIZE);

	head->size = 0;
	mid->size = ((PHY_END-PHY_BEGIN)>>PAGE_SIZE_SHIFT)-4;
	tail->size = 0;

	head->next = mid;
	mid->next = tail;
	tail->next = NULL;

	free_list = head;
}

char* alloc_pages(int num)
{
	run_t *last;
	for (run_t *p=free_list; p; p=p->next)
	{
		if (p->size>num)
		{
			p->size -= num;
			return (char *)p+(p->size<<PAGE_SIZE_SHIFT);
		}
		else if (p->size==num)
		{
			last->next = p->next;
			return (char *)p;
		}
		last = p;
	}

	uart_spin_puts("Error: alloc_pages\r\n");
	while (1);
}

void merge_pages(run_t *p)
{
	run_t *q = p->next;
	if ((char *)p+(p->size<<PAGE_SIZE_SHIFT)==(char *)q)
	{
		p->size += q->size;
		p->next = q->next;
	}
}

void free_pages(char *addr, int num)
{
	run_t *p = free_list;
	while ((char *)p->next < addr) p = p->next;
	
	run_t *new = (run_t *)addr;
	new->size = num;
	new->next = p->next;
	p->next = new;

	merge_pages(p);
	merge_pages(p);
}

void print_freelist()
{
	for (run_t *p=free_list; p; p=p->next)
	{
		puthex((u32)p);
		uart_spin_puts(" ");
		puthex((u32)p->size);
		uart_spin_puts(" ");
		puthex((u32)p->next);
		uart_spin_puts("\r\n");
	}
	uart_spin_puts("\r\n");
}

