#ifndef BTREE_H
#define BTREE_H
#include "page.h"
#include <stdbool.h>
#include "record.h"

#define NEXT_PAGE_INDEX -2

struct btree
{
    int order;
    int height;
    int root_page;
} __attribute__((packed));

void print_tree(struct btree *tree, const char* pages_filename);
void print_page_keys(struct page *p);
// returns true if key given was found in B-tree and sets address to record in data_address 
bool btree_search(const char* pages_filename, struct btree *tree, int key, int *data_address);
// saves given record in records file and indexes it in pages file, returns 0 if successfull
int btree_insert(const char* pages_filename, const char* records_filename, struct btree *tree, struct record *rec);

int btree_init(const char* pages_filename, const char* records_filename, int order, struct btree *tree);

#endif