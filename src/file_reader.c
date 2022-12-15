#include "file_reader.h"
#include <stdlib.h>

int create_files(const char* pages_filename, const char* records_filename, int order)
{
    FILE* pages = fopen(pages_filename, "wb");

    struct metadata data;
    data.height = 1;
    data.number_of_pages = 1;
    data.order = order;
    data.root_page_index = 0;

    if( fwrite(&data, sizeof(data), 1, pages) < 1)
    {
        printf("Failed to create pages file and add metadata:\n");
        return 1;
    }

    struct page *root_page = page_init(root_page, order);
    if( fwrite(root_page, PAGE_SIZE, 1, pages) < 1)
    {
        printf("Failed to add root_page to page file:\n");
        return 2;
    }

    fclose(pages);

    FILE* records = fopen(records_filename, "wb");
    fclose(records);

    return 0;
}

int set_metadata_height(const char* pages_filename, int height)
{
    FILE* f = fopen(pages_filename, "rb");

    struct metadata data;
    fseek(f, 0 ,0);

    if( fread(&data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 1;
    }

    fclose(f);

    data.height = height;

    f = fopen(pages_filename, "rb+");
    fseek(f, 0, 0);

    if( fwrite(&data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 2;
    }

    fclose(f);

    return 0;

}

int set_metadata_number_of_pages(const char* pages_filename, int n)
{
    FILE* f = fopen(pages_filename, "rb");

    struct metadata data;
    fseek(f, 0 ,0);

    if( fread(&data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 1;
    }

    fclose(f);

    data.number_of_pages = data.number_of_pages + n;

    f = fopen(pages_filename, "rb+");
    fseek(f, 0, 0);
    int s = fwrite(&data, sizeof(struct metadata), 1, f);
    if( s < 1)
    {
        printf("Failed to change metadata height\n");
        return 2;
    }
    fclose(f);

    return 0;
}

int get_number_of_pages(const char* pages_filename)
{
    struct metadata data;
    FILE* f = fopen(pages_filename, "rb");
    if(fread(&data, sizeof(struct record), 1, f) < 1)
    {
        printf("Failed to read number of records\n");
        return 1;
    }
    fclose(f);
    return data.number_of_pages;
}

int read_page(const char* pages_filename, struct page *p, int index, int order)
{
    FILE* f = fopen(pages_filename, "rb");
    fseek(f, sizeof(struct metadata) + (index * PAGE_SIZE), 0);
    if(fread(p, PAGE_SIZE, 1, f) < 1)
    {
        printf("Failed to read page at index %d\n", index);
        return 1;
    }
    fclose(f);
    return 0;
}

int save_page_at(const char* pages_filename, struct page *p, int index, int order)
{
    FILE* f = fopen(pages_filename, "rb+");
    fseek(f, sizeof(struct metadata) + (index * PAGE_SIZE), 0);
    if(fwrite(p, PAGE_SIZE, 1, f) < 1)
    {
        printf("Failed to write page at index %d\n", index);
        return 1;
    }
    fclose(f);

    return 0;
}

int save_page(const char* pages_filename, struct page *p, int order)
{
    FILE* f = fopen(pages_filename, "ab");
    if(fwrite(p, PAGE_SIZE, 1, f) < 1)
    {
        printf("Failed to write page at the end of file\n");
        return 1;
    }
    int index = (ftell(f) - sizeof(struct metadata))/PAGE_SIZE - 1;
    fclose(f);

    return index;
}

int read_metadata(const char* pages_filename, struct metadata *data)
{
    FILE* f = fopen(pages_filename, "rb");

    if(fread(data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to read metadata\n");
        return 1;
    }
    fclose(f);

    return 0;
}

int print_page_file(const char* pages_filename)
{
    FILE *f = fopen(pages_filename, "rb");

    struct metadata data;
    if( fread(&data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to print page file metadata\n");
        return 1;
    }
    int order = data.order;
    struct page *p = page_init(p, order);

    printf("Pages metadata:\n");
    printf("order=%d\t height=%d\tpages=%d\troot_page_ind=%d\n", data.order, data.height, data.number_of_pages, data.root_page_index);

    for(int i=0; i<data.number_of_pages; i++)
    {
        fseek(f, sizeof(struct metadata) + (i * PAGE_SIZE), 0);
        if(fread(p, PAGE_SIZE, 1, f) < 1)
        {
            printf("Failed to read page at index %d\n", i);
            return 2;
        }
        printf("Page %d: ", i);
        print_page_data(p);
    }

    free(p);
    fclose(f);

    return 0;
}

int set_metadata_root_index(const char* pages_filename, int index)
{
    FILE* f = fopen(pages_filename, "rb");

    struct metadata data;
    fseek(f, 0 ,0);

    if( fread(&data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to change metadata root\n");
        return 1;
    }

    fclose(f);

    data.root_page_index = index;

    f = fopen(pages_filename, "rb+");
    fseek(f, 0, 0);

    if( fwrite(&data, sizeof(struct metadata), 1, f) < 1)
    {
        printf("Failed to change metadata root\n");
        return 2;
    }

    fclose(f);

    return 0;
}