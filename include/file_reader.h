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
    int number_of_records;
} __attribute__((packed));

// creates pages b-tree file and main data file that contains all records, returns 0 if successfull
int create_files(const char* pages_filename, const char* records_filename, int order);
// sets height of b-tree in metadata, returns 0 if successfull
int set_metadata_height(const char* pages_filename, int height);
// sets number_of_records of b-tree in metadata, returns 0 if successfull
int set_metadata_number_of_records(const char* pages_filename, int n);
// saves record only to records file and returns its index in the file
long save_record(const char* records_filename, struct record *rec);
// loads record from file at given index, returns 0 if successfull
int load_record(const char* records_filename, struct record *rec, int index);
// saves record at given index, returns 0 if successfull
int save_record_at(const char* records_filename, struct record *rec, int index);
// returns the number of records from metadata
int get_number_of_records(const char* pages_filename);



// // creates new file with metadata only, returns 0 if successfull
// int create_new_file(const char* filename, int order);
// // sets number of pages in metadata of the file, returns 0 if successfull
// int set_number_of_pages(const char* filename, int n);
// // reads the record at desired address(read from page entries), returns 0 if successfull
// int read_record(FILE** file, struct record *rec, int address);
// // saves record in the page given, returns 0 if successfull
// int save_record(FILE** file, struct page *p, struct record *rec);

#endif
