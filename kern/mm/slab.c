#include <slab.h>
#include <config.h>
#include <kalloc.h>
#include <kernel.h>
#include <drivers/serial/uart.h>

void swap(u32 *a, u32 *b)
{
	u32 t = *a;
	*a = *b;
	*b = t;
}

char *align_addr(char *p, int shift)
{
	u32 base = (u32)((1<<shift)-1);
	return (char *)((((u32)p+base)&~base));
}

void init_slab_cache(slab_cache_t *cache, int obj_size, int obj_num, int align_shift, int pages_of_slab)
{
	cache->obj_size = obj_size;
	cache->obj_num = obj_num;
	cache->align_shift = align_shift;
	cache->pages_of_slab = pages_of_slab;
	cache->slab_list = NULL;
}

void alloc_slab(slab_cache_t *cache)
{
	slab_t *slab = (slab_t *)alloc_pages(cache->pages_of_slab);
	slab->obj_of_free = cache->obj_num;
	slab->next = cache->slab_list;
	cache->slab_list = slab;
	u32 *free_list = (u32 *)((char *)slab+sizeof(slab_t));
	char *p = align_addr((char *)free_list+(cache->obj_num<<2), cache->align_shift);
	for (int i=0; i<cache->obj_num; ++i)
		free_list[i] = (u32)(p+i*cache->obj_size);
}

void free_slab(slab_cache_t *cache, slab_t *slab)
{
	if (slab==cache->slab_list)
	{
		cache->slab_list = slab->next;
		free_pages((char *)slab, cache->pages_of_slab);
		return;
	}

	slab_t *last;
	for (slab_t *p=cache->slab_list; p!=NULL; p=p->next)
	{
		if (slab==p)
		{
			last->next = slab->next;
			free_pages((char *)slab, cache->pages_of_slab);
			return;
		}
		last = p;
	}

	uart_spin_puts("Error: free_slab\r\n");
	while (1);
}

char *alloc_obj(slab_cache_t *cache)
{
	while (1)
	{
		for (slab_t *slab=cache->slab_list; slab!=NULL; slab=slab->next)
		{
			if (slab->obj_of_free>0)
			{
				--slab->obj_of_free;
				u32 *free_list = (u32*)((char *)slab+sizeof(slab_t));
				return (char *)free_list[slab->obj_of_free];
			}
		}
		alloc_slab(cache);
	}
}

void free_obj(slab_cache_t *cache, void *addr)
{
	for (slab_t *slab=cache->slab_list; slab!=NULL; slab=slab->next)
		if ((void *)slab<addr && addr<(void *)slab+(cache->pages_of_slab<<PAGE_SIZE_SHIFT))
			for (int i=slab->obj_of_free; i<cache->obj_num; ++i)
			{
				u32 *free_list = (u32 *)((char *)slab+sizeof(slab_t));
				if (free_list[i]==(u32)addr)
				{
					swap(free_list+i, free_list+slab->obj_of_free);
					++slab->obj_of_free;
					if (slab->obj_of_free==cache->obj_num) free_slab(cache, slab);
					return;
				}
			}

	uart_spin_puts("Error: free_obj\r\n");
	while (1);
}

