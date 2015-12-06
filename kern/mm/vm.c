#define SLAB_SIZE 4

struct obj
{
	struct obj *next;
	int size;
}

struct slab
{
	struct slab *next;
	int obj_size;
} *slab_list;

void init_slab

char *alloc_obj(int size)
{

}

