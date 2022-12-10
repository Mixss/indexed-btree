#include "btree.h"
#include "stdlib.h"

void btree_init(struct btree *tree, int order)
{
    tree->order = order;
    tree->height = 1;
    tree->root_page = malloc(sizeof(struct page));
    page_init(tree->root_page, order);
}