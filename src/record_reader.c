#include "record_reader.h"
#include <stdbool.h>
#include <stdlib.h>

int record_write(const char* records_filename, struct record *rec)
{
    FILE *f = fopen(records_filename, "rb+");

    struct block *b = calloc(1, sizeof(struct block));
    bool new_block = true;
    int index = 0;
    int block_pos = 0;

    fseek(f, 0, SEEK_END);
    if( ftell(f) > 0 )
    {
        fseek(f, -sizeof(struct block), SEEK_END);
        block_pos = ftell(f);
        if( fread(b, sizeof(struct block), 1, f) < 1)
        {
            printf("Failed to read last block\n");
            return -1;
        }
        if (is_null(&b->records[RECORDS_IN_BLOCK-1]) == true)
            new_block = false;
    }

    if( new_block == true )
    {
        free(b);
        b = calloc(1, sizeof(struct block));
    }
    else // find the index in block to put new record 
    {
        for(int i=1; i<RECORDS_IN_BLOCK; i++)
        {
            if (is_null(&b->records[i]) == true)
            {
                index = i;
                break;
            }
        }

        if(index == 0)
        {
            printf("Something went wrong when looking for index in block\n");
            return -1;
        }
    }

    b->records[index] = *rec;

    // save block to the file
    if(new_block == true)
    {
        fseek(f, 0, SEEK_END);
        block_pos = ftell(f);
    }
    fseek(f, block_pos, 0);
    if( fwrite(b, sizeof(struct block), 1, f) < 1)
    {
        printf("Failed to write prepared block while saving the record\n");
        return -1;
    }

    free(b);
    fclose(f);

    return (block_pos / sizeof(struct block)) + index;
}

int record_read(const char* records_filename, struct record *rec, int index)
{
    FILE *f = fopen(records_filename, "rb");

    int block_nr = (index / RECORDS_IN_BLOCK);
    struct block b;

    fseek(f, block_nr * sizeof(struct block), 0);

    if( fread(&b, sizeof(struct block), 1, f) < 1)
    {
        printf("Failed to read block to read the record\n");
        return 1;
    }

    int index_in_block = index % RECORDS_IN_BLOCK;
    printf("Czytanie rekordu %d z bloku %d[%d]\n", index, block_nr, index_in_block);
    *rec = b.records[index_in_block];

    fclose(f);
    return 0;
}

int print_record_file(const char* records_filename)
{
    FILE *f = fopen(records_filename, "rb");
    struct record rec;
    printf("RECORDS FILE:\n");
    while(fread(&rec, sizeof(struct record), 1, f) > 0)
    {
        if(is_null(&rec) == true)
            printf("empty space (%ld bytes)\n", sizeof(rec));
        else print_record(&rec);
        
    }

    return 0;
}