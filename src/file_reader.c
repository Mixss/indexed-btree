#include "file_reader.h"

int create_files(const char* pages_filename, const char* records_filename, int order)
{
    FILE* pages = fopen(pages_filename, "wb");

    struct metadata data;
    data.height = 1;
    data.number_of_records = 0;
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

    if( fread(&data, sizeof(data), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 1;
    }

    fclose(f);

    data.height = height;

    f = fopen(pages_filename, "wb");
    fseek(f, 0, 0);

    if( fwrite(&data, sizeof(data), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 2;
    }

    fclose(f);

    return 0;

}

int set_metadata_number_of_records(const char* pages_filename, int n)
{
    FILE* f = fopen(pages_filename, "rb");

    struct metadata data;

    if( fread(&data, sizeof(data), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 1;
    }

    fclose(f);

    data.number_of_records = n;

    f = fopen(pages_filename, "wb");
    fseek(f, 0, 0);

    if( fwrite(&data, sizeof(data), 1, f) < 1)
    {
        printf("Failed to change metadata height\n");
        return 2;
    }

    fclose(f);

    return 0;
}

long save_record(const char* records_filename, struct record *rec)
{
    FILE* f = fopen(records_filename, "ab");
    if(fwrite(rec, sizeof(struct record), 1, f) < 1)
    {
        printf("Failed to append new record with id: %d\n", rec->id);
        return 1;
    }

    int index = ftell(f)/sizeof(struct record) - 1;
    fclose(f);

    return index;
}

int load_record(const char* records_filename, struct record *rec, int index)
{
    FILE* f = fopen(records_filename, "rb");
    if(f == NULL)
    {
        printf("Failed to load record (open file)\n");
        return 1;
    }

    fseek(f, index * sizeof(struct record), SEEK_SET);

    if( fread(rec, sizeof(struct record), 1, f) < 1)
    {
        printf("Failed to read record at index: %d\n", index);
        fclose(f);
        return 2;
    }

    fclose(f);

    return 0;
}

int save_record_at(const char* records_filename, struct record *rec, int index)
{
    FILE* f = fopen(records_filename, "wb");
    fseek(f, index * sizeof(struct record), 0);
    if(fwrite(rec, sizeof(struct record), 1, f) < 1)
    {
        printf("Failed to append new record with id: %d\n", rec->id);
        return 1;
    }
    fclose(f);

    return 0;
}

int get_number_of_records(const char* pages_filename)
{
    struct metadata data;
    FILE* f = fopen(pages_filename, "rb");
    if(fread(&data, sizeof(struct record), 1, f) < 1)
    {
        printf("Failed to read number of records\n");
        return 1;
    }
    fclose(f);

    return 0;
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
    FILE* f = fopen(pages_filename, "wb");
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
