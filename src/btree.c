#include "btree.h"
#include "file_reader.h"
#include "access_stats.h"
#include "stack.h"
#include <stdlib.h>
#include "record_reader.h"

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

int insert_in_page(const char* pages_filename, const char* records_filename, struct record *rec, struct page *p, struct btree *tree, int page_index)
{
    if(p->records_on_page == tree->order * 2)
    {
        printf("Tried to insert key to full page\n");
        return 1;
    }

    int insert_pos = -1;

    for(int i=0; i<p->records_on_page; i++)
    {
        if(rec->id < p->entries[i].key) // put the record before this entry
        {
            insert_pos = i;
            break;
        }
    }
    if(insert_pos == -1)
        insert_pos = p->records_on_page;

    for(int i=p->records_on_page; i>=insert_pos; i--)
    {
        p->entries[i] = p->entries[i-1];
    }

    int ind = record_write(records_filename, &rec);

    struct page_entry pe;
    pe.key = rec->id;
    pe.address_to_data = ind;
    pe.other_page = NIL;

    p->entries[insert_pos] = pe;
    p->records_on_page = p->records_on_page + 1;

    save_page_at(pages_filename, p, page_index, tree->order);

    return 0;
}

int insert_find_page_candidate(const char* pages_filename, struct btree *tree, struct page *p, int key)
{
    int page_index = tree->root_page;
    int order = tree->order;

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

    return page_index;
}

void insert_find_siblings(const char* pages_filename, struct btree *tree, struct page *p, int *left_sibling_index, int *right_sibling_index)
{
    *left_sibling_index = NIL;
    int left_sibling_index_in_root_page = 0;
    *right_sibling_index = NIL;

    int order = tree->order;

    struct page *root = page_init(root, order);

    read_page(pages_filename, root, p->parent_page, order);

    // iterate through root page to find left sibling
    for(int i=0; i<root->records_on_page; i++)
    {
        if(root->entries[i].key < p->entries[0].key) // it doesn't matter with wich key we compare
        {
            *left_sibling_index = root->entries->other_page;
            left_sibling_index_in_root_page = i;
            break;
        }
    }
    // if left s. index is still NIL it means that there is no left sibling

    printf("Lewy sąsiad: %d\n", *left_sibling_index);

    // find right sibling

    if(root->entries[root->records_on_page-1].key > p->entries[0].key) // if that is false it means that there is no right sibling
    {
        if(*left_sibling_index == -1) // if there is no left sibling we can set this variable that it'll be correct in next computations
            left_sibling_index_in_root_page = -1;

        if(left_sibling_index_in_root_page+2 >= root->records_on_page) // right sibling is in next_page
            *right_sibling_index = root->next_page;
        else *right_sibling_index = root->entries[left_sibling_index_in_root_page+2].other_page;
    }

    printf("Prawy sąsiad: %d\n", *right_sibling_index);

    free(root);
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

    int page_index = insert_find_page_candidate(pages_filename, tree, p, key);
    
    // check if basic insertion to the page is possible
    if(p->records_on_page < tree->order * 2) // there is vacancy in the page
    {
        insert_in_page(pages_filename, records_filename, rec, p, tree, page_index);
        goto end;
    }

    // check if compensation is possible

    // FIND SIBLINGS
    struct page *left_sibling = page_init(left_sibling, order);
    struct page *right_sibling = page_init(right_sibling, order);

    int left_sibling_index;
    int right_sibling_index;

    insert_find_siblings(pages_filename, tree, p, &left_sibling_index, &right_sibling_index);

    free(left_sibling);
    free(right_sibling);

    end:

    free(p);
    return 0;
}
