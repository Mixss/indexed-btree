#include "page.h"
#include <stdlib.h>
#include <stdio.h>

struct page *page_init(struct page *p, int order)
{
    p = malloc( PAGE_SIZE );
    p->records_on_page = 0;
    p->next_page = NIL;
    p->parent_page = NIL;
    p->is_leaf = true;
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

void print_page_data(struct page *p)
{
    printf("Parent ind.: %d\tGreater keys ind.: %d\tRecords: %d\n", p->parent_page, p->next_page, p->records_on_page);
    for(int i=0; i<p->records_on_page; i++)
    {
        printf("\tPage entry: key=%d\taddr=%d\tlesser_keys_page=%d\n", p->entries[i].key, p->entries[i].address_to_data, p->entries[i].other_page);
    }
}