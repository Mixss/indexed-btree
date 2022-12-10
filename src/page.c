#include "page.h"
#include "stdlib.h"

void page_init(struct page *p, int order)
{
    p->records_on_page = 0;
    p->keys = malloc(2 * order * sizeof(int));
    p->pages = malloc(2 * order * sizeof(struct page*));
    p->address = malloc(2 * order * sizeof(void*));
}

void page_free(struct page *p)
{
    free(p->keys);
    free(p->pages);
    free(p->address);
}