#include <stdio.h>
#include <stdlib.h>
#include "file_reader.h"
#include "page.h"
#include "btree.h"
#include "record_reader.h"
#include "access_stats.h"

void save_example_tree(const char* pages_filename, const char* records_filename, struct btree *tree)
{
    int order = 2;

    create_files(pages_filename, records_filename, order);

    struct page *p1 = page_init(p1, order);
    struct page *p2 = page_init(p2, order);
    struct page *p3 = page_init(p3, order);
    
    struct record rec;
    record_set(&rec, 5, 0, 0, 0, 0, 0, 0);
    int ind = record_write(records_filename, &rec);
    set_entry(p1, 0, rec.id, ind, NIL);
    
    record_set(&rec, 7, 1, 1, 1, 1, 1, 1);
    ind = record_write(records_filename, &rec);
    set_entry(p1, 1, rec.id, ind, NIL);

    record_set(&rec, 10, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p1, 2, rec.id, ind, NIL);
    p1->records_on_page = 3;
    p1->parent_page = 0;
    // p2
    record_set(&rec, 18, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p2, 0, rec.id, ind, NIL);

    record_set(&rec, 19, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p2, 1, rec.id, ind, NIL);
    p2->records_on_page = 2;
    p2->parent_page = 0;
    // p3
    record_set(&rec, 25, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p3, 0, rec.id, ind, NIL);

    record_set(&rec, 27, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p3, 1, rec.id, ind, NIL);

    record_set(&rec, 30, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p3, 2, rec.id, ind, NIL);

    p3->records_on_page = 3;
    p3->parent_page = 0;
    // root page
    struct page *p = page_init(p, order);
    struct metadata data;
    read_metadata(pages_filename, &data);

    read_page(pages_filename, p, data.root_page_index, order);
    int page_ind = save_page(pages_filename, p1, order);
    record_set(&rec, 15, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p, 0, rec.id, ind, page_ind);
    p->records_on_page = 2;
    p->is_leaf = false;

    page_ind = save_page(pages_filename, p2, order);
    record_set(&rec, 22, 2, 2, 2, 2, 2, 2);
    ind = record_write(records_filename, &rec);
    set_entry(p, 1, rec.id, ind, page_ind);  

    page_ind = save_page(pages_filename, p3, order);
    p->next_page = page_ind;

    save_page_at(pages_filename, p, 0, order);

    tree->root_page = data.root_page_index;
    
    set_metadata_height(pages_filename, 2);
    set_metadata_number_of_pages(pages_filename, 3);
    
    page_free(p1);
    page_free(p2);
    page_free(p3);
    page_free(p);
}

/*
void old_main()
{
    save_example_tree(pages_filename, records_filename, &tree);
    printf("===================\n");
    //print_page_file(pages_filename);
    print_tree(&tree, pages_filename);
    //print_page_file(pages_filename);

    int data_address;
    struct record rec;

    record_set(&rec, 1, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 2, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    record_set(&rec, 3, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);


    record_set(&rec, 35, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    print_tree(&tree, pages_filename);

    record_set(&rec, 4, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    record_set(&rec, 40, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    record_set(&rec, 45, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    record_set(&rec, 28, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    record_set(&rec, 29, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    record_set(&rec, 32, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    print_tree(&tree, pages_filename);

    record_set(&rec, 33, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    print_tree(&tree, pages_filename);

    // record_set(&rec, 17, 1, 2, 3, 4, 5, 6);
    // btree_insert(pages_filename, records_filename, &tree, &rec);

    print_page_file(pages_filename);
}
*/
int main(int argc, char **argv)
{
    const char* pages_filename = "./data/pages";
    const char* records_filename = "./data/records";

    struct btree tree;
    tree.order = 2;

    btree_init(pages_filename, records_filename, 2, &tree);

    // save_example_tree(pages_filename, records_filename, &tree);
    // printf("===================\n");
    // print_page_file(pages_filename);
    // //print_page_file(pages_filename);
    // print_tree(&tree, pages_filename);
    // int data_address;
    // printf("%d\n", btree_search_corn(pages_filename, &tree, 30, &data_address));
    // printf("%d\n", btree_search_corn(pages_filename, &tree, 19, &data_address));
    // printf("%d\n", btree_search_corn(pages_filename, &tree, 22, &data_address));

    // char buffer[100];
    char command;

    // goto jazda;

    struct record rec;

    record_set(&rec, 100, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 200, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 300, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 400, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 500, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);    
    record_set(&rec, 600, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 700, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 800, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 900, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1000, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1100, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1200, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1300, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1500, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1600, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1700, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1800, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1900, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 2000, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1650, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1740, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 1780, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 2100, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);
    record_set(&rec, 2200, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    print_page_file(pages_filename);

    record_set(&rec, 1250, 1, 2, 3, 4, 5, 6);
    btree_insert(pages_filename, records_filename, &tree, &rec);

    print_page_file(pages_filename);

    return 0;

    jazda:

    while(scanf("%c", &command) != EOF)
    {
        if(command == 'a')
        {
            struct record rec;
            int key;
            scanf("%d", &key);
            printf("Add: %d\n", key);

            record_set(&rec, key, 1, 2, 3, 4, 5, 6);
            btree_insert(pages_filename, records_filename, &tree, &rec);
        }
        else if(command == 'p')
        {
            print_page_file(pages_filename);
        }
        else if(command == 'r')
        {
            print_record_file(records_filename);
        }
        else if(command == 's')
        {
            int data_address;
            int key;
            scanf("%d", &key);
            if(btree_search(pages_filename, &tree, key, &data_address))
            {
                struct record rec;
                record_read(records_filename, &rec, data_address);
                print_record(&rec);
            }
            else
                printf("Record with key %d was not found\n", key);

        }
    }

    return 0;
}