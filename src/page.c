#include "page.h"
#include <stdlib.h>

struct page *page_init(struct page *p, int order)
{
    p = malloc( sizeof(*p) + sizeof(struct page_entry) * 2 * order );
    p->records_on_page = 0;

    return p;
}

void page_free(struct page *p)
{
    free(p);
}