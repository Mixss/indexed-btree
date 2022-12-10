#ifndef PAGE_H
#define PAGE_H

struct page
{
    int records_on_page;
    struct page **pages;
    int *keys;
    void **address;
} __attribute__((packed)); 

void page_init(struct page *p, int order);
void page_free(struct page *p);

#endif