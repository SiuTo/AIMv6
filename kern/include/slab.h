#ifndef _SLAB_H
#define _SLAB_H

#include <sys/types.h>

typedef struct slab
{
	int obj_of_free;
	struct slab *next;
} slab_t;

typedef struct slab_cache
{
	int obj_size;
	int obj_num;
	int align_shift;
	int pages_of_slab;
	slab_t *slab_list;
} slab_cache_t;

void init_slab_cache(slab_cache_t *cache, int obj_size, int obj_num, int align_shift, int pages_of_slab);
char *alloc_obj(slab_cache_t *cache);
void free_obj(slab_cache_t *cache, void *addr);

#endif

