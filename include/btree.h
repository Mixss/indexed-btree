#ifndef BTREE_H
#define BTREE_H
#include "page.h"

struct btree
{
    int order;
    int height;
    struct page *root_page;
} __attribute__((packed));

void btree_init(struct btree *tree, int order);

#endif