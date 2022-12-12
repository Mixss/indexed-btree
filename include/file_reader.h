#ifndef FILE_READER_H
#define FILE_READER_H
#include <stdio.h>
#include "record.h"
#include "page.h"

#define RECORDS_FILENAME "./data/records"
#define PAGES_FILENAME "./data/pages"

struct metadata
{
    int height;
    int order;
    int number_of_pages;
    int root_page_index;
} __attribute__((packed));

// creates pages b-tree file and main data file that contains all records, returns 0 if successfull
int create_files(const char* pages_filename, const char* records_filename, int order);
// sets height of b-tree in metadata, returns 0 if successfull
int set_metadata_height(const char* pages_filename, int height);
// sets number_of_records of b-tree in metadata, returns 0 if successfull
int set_metadata_number_of_pages(const char* pages_filename, int n);
// returns the number of records from metadata
int get_number_of_pages(const char* pages_filename);

// reads page from page file at given index, returns 0 if successfull
int read_page(const char* pages_filename, struct page *p, int index, int order);
// saves the page at the index given, returns 0 if successfull
int save_page_at(const char* pages_filename, struct page *p, int index, int order);
// saves the page in the file and returns its index
int save_page(const char* pages_filename, struct page *p, int order);

// reads metadata, returns 0 if successfull
int read_metadata(const char* pages_filename,struct metadata *data);
// prints page file with all its data, returns 0 if successfull
int print_page_file(const char* pages_filename);




#endif
