#include "btree.h"
#include "file_reader.h"

void print_tree(struct btree *tree, const char* pages_filename)
{
    printf("B-tree structure:\n\n");
    struct page *p = page_init(p, tree->order);
    read_page(pages_filename, p, tree->root_page, tree->order);
    printf("Page entries %d\n", p->records_on_page);
    print_page_keys(p);
    printf("\n");
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
