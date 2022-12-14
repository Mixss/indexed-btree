#ifndef PAGE_H
#define PAGE_H

#define PAGE_SIZE (sizeof(struct page) + sizeof(struct page_entry) * 2 * order)
#define NIL -1

#include <stdbool.h>

struct page_entry
{
    int other_page; //index in the page_file of the page containing lesser keys
    int key;
    int address_to_data;
} __attribute__((packed));

struct page
{
    int records_on_page;
    int parent_page;
    bool is_leaf;
    int next_page; //index in the page_file of the page containing greater keys than last record in this page
    struct page_entry entries[];
} __attribute__((packed)); 

struct page *page_init(struct page *p, int order);
void page_free(struct page *p);
void set_entry(struct page *p, int entry_index, int key, int address_to_data, int other_page);
void print_page_data(struct page *p);

void page_set_array_at(struct page **array, int index, struct page *p, int order);
void page_get_array_at(struct page **array, int index, struct page *p, int order);

void page_copy(struct page *from, struct page *to);

#endif