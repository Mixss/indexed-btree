#include "page.h"
#include <stdlib.h>

struct page *page_init(struct page *p, int order)
{
    p = malloc( PAGE_SIZE );
    p->records_on_page = 0;

    return p;
}

void page_free(struct page *p)
{
    free(p);
}