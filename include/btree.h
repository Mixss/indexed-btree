#ifndef BTREE_H
#define BTREE_H
#include "page.h"

struct btree
{
    int order;
    int height;
    int root_page;
} __attribute__((packed));

void print_tree(struct btree *tree, const char* pages_filename);
void print_page_keys(struct page *p);

#endif