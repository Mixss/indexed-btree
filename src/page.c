#include "page.h"
#include <stdlib.h>

struct page *page_init(struct page *p, int order)
{
    p = malloc( PAGE_SIZE );
    p->records_on_page = 0;
    p->next_page = NIL;
    p->parent_page = NIL;
    return p;
}

void page_free(struct page *p)
{
    free(p);
}

void set_entry(struct page *p, int entry_index, int key, int address_to_data, int other_page)
{
    p->entries[entry_index].key = key;
    p->entries[entry_index].address_to_data = address_to_data;
    p->entries[entry_index].other_page = other_page;
}