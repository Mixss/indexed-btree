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
    p->address = NIL;
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
    printf("Parent ind.: %d\tGreater keys ind.: %d\tRecords: %d\t Address: %d\n", p->parent_page, p->next_page, p->records_on_page, p->address);
    for(int i=0; i<p->records_on_page; i++)
    {
        printf("\tPage entry: key=%d\taddr=%d\tlesser_keys_page=%d\n", p->entries[i].key, p->entries[i].address_to_data, p->entries[i].other_page);
    }
}

void page_set_array_at(struct page **array, int index, struct page *p, int order)
{
    struct page *ptr = (array + (index * PAGE_SIZE));
    ptr = p;
}

void page_get_array_at(struct page **array, int index, struct page *p, int order)
{
    p = (struct page*)(array + (index * PAGE_SIZE));
}

void page_copy(struct page *from, struct page *to)
{
    to->is_leaf = from->is_leaf;
    to->next_page = from->next_page;
    to->parent_page = from->parent_page;
    to->records_on_page = from->records_on_page;
    for(int i=0; i<to->records_on_page; i++)
    {
        to->entries[i] = from->entries[i];
    }
}