#ifndef RECORD_READER_H
#define RECORD_READER_H

#define RECORDS_IN_BLOCK 4

#include "record.h"

struct block
{
    struct record records[RECORDS_IN_BLOCK];
} __attribute__((packed));

// saves the record in the file and returns ints index
int record_write(const char* records_filename, struct record *rec);
// loads the record from file at given index, returns 0 if successfull
int record_read(const char* records_filename, struct record *rec, int index);


#endif