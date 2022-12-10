#ifndef PAGE_H
#define PAGE_H

struct page_entry
{
    struct page *other_page;
    int key;
    int address_to_data;
} __attribute__((packed));

struct page
{
    int records_on_page;
    struct page_entry entries[];
} __attribute__((packed)); 

struct page *page_init(struct page *p, int order);
void page_free(struct page *p);

#endif