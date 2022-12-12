#ifndef BTREE_H
#define BTREE_H
#include "page.h"
#include <stdbool.h>

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

#endif