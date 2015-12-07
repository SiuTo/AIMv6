#ifndef _KALLOC
#define _KALLOC

void init_freelist();
char *alloc_pages(int num);
void free_pages(char *addr, int num);
void print_freelist();

#endif

