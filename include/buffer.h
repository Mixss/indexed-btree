#ifndef BUFFER_H
#define BUFFER_H

struct page_buffer
{
    struct page_buffer_element *current;
    int length;
    int index;
};

struct page_buffer_element
{
    struct page **p;
    struct page_buffer_element *next;
    struct page_buffer_element *prev;
};

void page_buffer_init(struct page_buffer *buffer);
void page_buffer_add_element(struct page_buffer *buffer, struct page **p);
void page_buffer_get_element(struct page_buffer *buffer, struct page **p, int index);
int page_buffer_length(struct page_buffer *buffer);
void page_buffer_free(struct page_buffer *buffer);
void page_buffer_next(struct page_buffer *buffer);
void page_buffer_prev(struct page_buffer *buffer);

#endif