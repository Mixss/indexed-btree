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
    printf("Zapisywanie bloku na: %d\n", block_pos);
    fseek(f, block_pos, 0);
    if( fwrite(b, sizeof(struct block), 1, f) < 1)
    {
        printf("Failed to write prepared block while saving the record\n");
        return -1;
    }

    free(b);
    fclose(f);

    return (block_pos / RECORDS_IN_BLOCK) + index;
}