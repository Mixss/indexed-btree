#include "btree.h"
#include "file_reader.h"
#include "access_stats.h"
#include "stack.h"
#include <stdlib.h>

void print_tree(struct btree *tree, const char* pages_filename)
{
    printf("B-tree structure:\n\n");
    struct page *p = page_init(p, tree->order);
    //read_page(pages_filename, p, tree->root_page, tree->order);

    struct stack st;
    struct stack st2;
    stack_clear(&st);
    stack_clear(&st2);
    
    stack_push(&st, tree->root_page);

    while(stack_len(&st) > 0)
    {
        while(stack_len(&st) > 0)
        {
            int ind = stack_pop_base(&st);
            read_page(pages_filename, p, ind, tree->order);

            print_page_keys(p);

            for(int i=0; i<p->records_on_page; i++)
            {
                if(p->entries[i].other_page != NIL)
                    stack_push(&st2, p->entries[i].other_page);
            }
            if(p->next_page != NIL)
                stack_push(&st2, p->next_page);
                printf("  ");
        }
        st = st2;
        stack_clear(&st2);    
        printf("\n\n");
    }
    
    free(p);
}

void print_page_keys(struct page *p)
{
    printf("|");
    for(int i=0; i<p->records_on_page; i++)
    {
        printf("|%d", p->entries[i].key);
    }
    printf("||");
}

bool btree_search(const char* pages_filename, struct btree *tree, int key, int *data_address)
{
    int order = tree->order;
    struct page *p = page_init(p, order);

    int page_index = tree->root_page;
    bool found = false;

    while(1)
    {
        if(page_index == NIL)
            break;
        
        read_page(pages_filename, p, page_index, order);
        add_disk_read(1);
        
        // lesser than the smallest
        if(key < p->entries[0].key)
        {
            page_index = p->entries[0].other_page;
            continue;
        }
        // equal to one in this page
        for(int i=0; i<p->records_on_page; i++)
        {
            if(key == p->entries[i].key)
            {
                found = true;
                *data_address = p->entries[i].address_to_data;
                break;
            }
        }
        if(found == true) break;
        //greater than the largest
        if(key > p->entries[p->records_on_page-1].key)
        {
            page_index = p->next_page;
            continue;
        }
        // must be between keys on this page
        for(int i=1; i<p->records_on_page; i++)
        {
            if(key < p->entries[i].key)
            {
                page_index = p->entries[i].other_page;
                break;
            }
        }
    }

    free(p);
    return found;
}

int btree_insert(const char* pages_filename, const char* records_filename, struct btree *tree, struct record *rec)
{
    int key = rec->id;
    int data_address;
    if (btree_search(pages_filename, tree, key, &data_address) == true)
    {
        printf("Insert: given key is already present\n");
        return 1;
    }

    // find page that will be a candidate to insert key in it

    int order = tree->order;
    struct page *p = page_init(p, order);

    int page_index = tree->root_page;

    while(1)
    {
        read_page(pages_filename, p, page_index, order);
        add_disk_read(1);

        if(p->is_leaf == true)
            break;
        
        // lesser than the smallest
        if(key < p->entries[0].key)
        {
            page_index = p->entries[0].other_page;
            continue;
        }
        // greater than the biggest
        else if(key > p->entries[p->records_on_page - 1].key)
        {
            page_index = p->next_page;
            continue;
        }
        // between
        for(int i=1; i<p->records_on_page; i++)
        {
            if(key < p->entries[i].key)
            {
                page_index = p->entries[i].other_page;
                break;
            }
        }
    }

    printf("Record could fit in %d\n", page_index);

    free(p);
    return 0;
}
