#include "buffer.h"
#include <stdlib.h>

void page_buffer_init(struct page_buffer *buffer)
{
    buffer->current = malloc(sizeof(struct page_buffer_element));
    buffer->current->p = NULL;
    buffer->current->next = NULL;
    buffer->current->prev = NULL;
    buffer->index = 0;
    buffer->length = 0;
}

void page_buffer_add_element(struct page_buffer *buffer, struct page **p)
{
    if(buffer->length == 0)
    {
        buffer->current->p = p;
        buffer->length = buffer->length + 1;
        return;
    }
    struct page_buffer_element *pbe = malloc(sizeof(struct page_buffer_element));
    pbe->prev = buffer->current;
    pbe->next = NULL;
    pbe->p = p;

    buffer->current->next = pbe;
    buffer->length = buffer->length + 1;
    buffer->current = pbe;
}

void page_buffer_next(struct page_buffer *buffer)
{
    if(buffer->current->next == NULL)
        return;
    buffer->current = buffer->current->next;
    buffer->index = buffer->index + 1;
}
void page_buffer_prev(struct page_buffer *buffer)
{
    if(buffer->current->prev == NULL)
        return;
    buffer->current = buffer->current->prev;
    buffer->index = buffer->index - 1;
}

void page_buffer_get_element(struct page_buffer *buffer, struct page **p, int index)
{
    if(index > buffer->index)
        while(index != buffer->index)
            page_buffer_next(buffer);
    else if(index < buffer->index)
        while(index != buffer->index)
            page_buffer_prev(buffer);
    p = buffer->current->p;
}

int page_buffer_length(struct page_buffer *buffer)
{
    return buffer->length;
}
void page_buffer_free(struct page_buffer *buffer)
{
    struct page **p;
    page_buffer_get_element(buffer, p, 0);

    while(buffer->current != NULL)
    {
        struct page_buffer_element *pbe = buffer->current;
        page_buffer_next(buffer);
        free(pbe);
    }
}