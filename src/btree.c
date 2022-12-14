#include "btree.h"
#include "file_reader.h"
#include "access_stats.h"
#include "stack.h"
#include <stdlib.h>
#include "record_reader.h"
#include "buffer.h"

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

    int ind = record_write(records_filename, rec);

    struct page_entry pe;
    pe.key = rec->id;
    pe.address_to_data = ind;
    pe.other_page = NIL;

    p->entries[insert_pos] = pe;
    p->records_on_page = p->records_on_page + 1;

    save_page_at(pages_filename, p, page_index, tree->order);

    return 0;
}

int insert_find_page_candidate(const char* pages_filename, struct btree *tree, struct page *page_candidate, int key, struct page_buffer *buffer)
{
    int page_index = tree->root_page;
    int order = tree->order;
    page_buffer_init(buffer);
    printf("KURWA\n");

    while(1)
    {
        struct page *p = page_init(p, order);
        read_page(pages_filename, p, page_index, order);
        add_disk_read(1);

        page_buffer_add_element(buffer, &p);

        if(p->is_leaf == true)
        {
            page_copy(p, page_candidate);
            break;
        }
            
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

void insert_find_siblings(const char* pages_filename, struct btree *tree, struct page *p, int *left_sibling_index, int *right_sibling_index, struct page *parent_page)
{
    *left_sibling_index = NIL;
    int left_sibling_index_in_root_page = 0;
    *right_sibling_index = NIL;

    int order = tree->order;

    read_page(pages_filename, parent_page, p->parent_page, order);

    // iterate through root page to find left sibling
    for(int i=parent_page->records_on_page-1; i>=0; i--)
    {
        if(parent_page->entries[i].key < p->entries[0].key) // it doesn't matter with wich key we compare
        {
            *left_sibling_index = parent_page->entries[i].other_page;
            left_sibling_index_in_root_page = i;
            break;
        }
    }
    // if left s. index is still NIL it means that there is no left sibling

    // find right sibling

    if(parent_page->entries[parent_page->records_on_page-1].key > p->entries[0].key) // if that is false it means that there is no right sibling
    {
        if(*left_sibling_index == -1) // if there is no left sibling we can set this variable that it'll be correct in next computations
            left_sibling_index_in_root_page = -1;

        if(left_sibling_index_in_root_page+2 >= parent_page->records_on_page) // right sibling is in next_page
            *right_sibling_index = parent_page->next_page;
        else *right_sibling_index = parent_page->entries[left_sibling_index_in_root_page+2].other_page;
    }
}

void insert_compensation(const char* pages_filename, const char* records_filename, int left_compensation, int left_sibling_index, int right_sibling_index, struct page *left_sibling, struct page *right_sibling, int page_index, struct page *parent_page, struct record *rec, struct btree *tree, struct page *p)
{
    //get all records that will take part in compensation
        // we want only one variable to operate on sibling
        int sibling_index;
        struct page **sibling;

        if(left_compensation)
        {
            sibling_index = left_sibling_index;
            sibling = &left_sibling;
        }
        else
        {
            sibling_index = right_sibling_index;
            sibling = &right_sibling;
        }

        // sum of records on both pages, parent page and new record page
        int number_of_records = (*sibling)->records_on_page + p->records_on_page + 2; 

        struct page_entry *compensation_entries = calloc(number_of_records, sizeof(struct page_entry));

        // fill the array
        int sibling_offset = left_compensation == true ? 0 : p->records_on_page + 1;
        int compensation_count = 0;

        int page_offset = left_compensation == true ? (*sibling)->records_on_page + 1 : 0;

        for(int i=0; i<(*sibling)->records_on_page; i++)
            compensation_entries[i+sibling_offset] = (*sibling)->entries[i];

        for(int i=0; i<p->records_on_page; i++, compensation_count++)
            compensation_entries[i + page_offset] = p->entries[i];

        //find parent entry
        int parent_entry_cmp_index = left_compensation == true ? (*sibling)->records_on_page : p->records_on_page;
        int left_parent_key = compensation_entries[parent_entry_cmp_index-1].key;
        int parent_entry_index_in_parent_page;
        struct page_entry parent_entry;

        for(int i=0; i<parent_page->records_on_page; i++)
        {
            if(parent_page->entries[i].key > left_parent_key)
            {
                parent_entry = parent_page->entries[i];
                parent_entry_index_in_parent_page = i;
                break;
            }
        }
        // we have to clear other page info, because parent entry will go to the leaf page
        int parent_entry_other_page = parent_entry.other_page;
        parent_entry.other_page = NIL;

        compensation_entries[parent_entry_cmp_index] = parent_entry;
        // save new record to disk
        struct page_entry new_record_entry;
        new_record_entry.address_to_data = record_write(records_filename, rec);
        new_record_entry.key = rec->id;
        new_record_entry.other_page = NIL;

        // place new entry in array, finding place is trivial, array is sorted;
        for(int i=number_of_records-1; i>0; i--)
        {
            if(compensation_entries[i-1].key < rec->id)
            {
                for(int j=number_of_records - 2; j>=i; j--)
                {
                    compensation_entries[j + 1] = compensation_entries[j];
                }

                compensation_entries[i] = new_record_entry;
                break;
            }
        }

        int new_parent_entry_index = number_of_records / 2;

        parent_entry = compensation_entries[new_parent_entry_index];
        parent_entry.other_page = parent_entry_other_page;

        // put new data to pages
        (*sibling)->records_on_page = 0;
        p->records_on_page = 0;

        parent_page->entries[parent_entry_index_in_parent_page] = parent_entry;

        int i;
        if(left_compensation)
        {
            for(i=0; i<number_of_records / 2; i++)
                (*sibling)->entries[i] = compensation_entries[i];
            (*sibling)->records_on_page = i;
            for(i=0; i<number_of_records - new_parent_entry_index - 1; i++)
                p->entries[i] = compensation_entries[new_parent_entry_index + i + 1];
            p->records_on_page = i;
        }
        else
        {
            for(i=0; i<number_of_records / 2; i++)
                p->entries[i] = compensation_entries[i];
            p->records_on_page = i;
            for(i=0; i<number_of_records - new_parent_entry_index - 1; i++)
                (*sibling)->entries[i] = compensation_entries[new_parent_entry_index + i + 1];
            (*sibling)->records_on_page = i;
        }
        
        //save updated pages
        save_page_at(pages_filename, *sibling, sibling_index, tree->order);
        save_page_at(pages_filename, p, page_index, tree->order);
        save_page_at(pages_filename, parent_page, p->parent_page, tree->order);


        free(compensation_entries);
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
    struct page_buffer buffer;

    int page_index = insert_find_page_candidate(pages_filename, tree, p, key, &buffer);
    
    // check if basic insertion to the page is possible
    if(p->records_on_page < tree->order * 2) // there is vacancy in the page
    {
        insert_in_page(pages_filename, records_filename, rec, p, tree, page_index);
        goto end;
    }

    // CHECK IF COMPENSATION IS POSSIBLE

    bool left_compensation = false;
    bool right_compensation = false;

    // FIND SIBLINGS
    int left_sibling_index = NIL;
    int right_sibling_index = NIL;

    struct page *parent_page = page_init(parent_page, tree->order);

    insert_find_siblings(pages_filename, tree, p, &left_sibling_index, &right_sibling_index, parent_page);
    // reset all entries other_page to NIL - when compensation occur parent record will be changed

    struct page *left_sibling = page_init(left_sibling, order);
    struct page *right_sibling = page_init(right_sibling, order);

    read_page(pages_filename, left_sibling, left_sibling_index, tree->order);
    read_page(pages_filename, right_sibling, right_sibling_index, tree->order);

    if(left_sibling_index != NIL)
        if(left_sibling->records_on_page < tree->order * 2) // copensation is possible with left neighbour
            left_compensation = true;
    
    if(right_sibling_index != NIL)
        if(right_sibling->records_on_page < tree->order * 2) // copensation is possible with right neighbour
            right_compensation = true;

    // COMPENSATION
    
    if(left_compensation || right_compensation)
        insert_compensation(pages_filename, records_filename, left_compensation, left_sibling_index, right_sibling_index, left_sibling, right_sibling, page_index, parent_page, rec, tree, p);
   

    printf("SPLIT!\n");
    

    end_cmp:

    free(parent_page);
    free(left_sibling);
    free(right_sibling);

    end:

    free(p);
    return 0;
}
