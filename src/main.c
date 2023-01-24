#include <stdio.h>
#include <stdlib.h>
#include "file_reader.h"
#include "page.h"
#include "btree.h"
#include "record.h"
#include "record_reader.h"
#include <string.h>
#include "access_stats.h"

#define INPUT_TEXT_FILE "input.txt"

void text_mode(const char* pages_filename, const char* records_filename, struct btree *tree)
{
    FILE* file = fopen(INPUT_TEXT_FILE, "r");
    if(file == NULL)
    {
        printf("Failed to open text file.\n");
        return;
    }

    char command;
    int sum = 0;
    int sumw = 0;
    int cnt = 0;

    while(fscanf(file, "%c", &command) != EOF)
    {
        if(command == 'a')
        {
            struct record rec;
            int key;
            fscanf(file, "%d", &key);
            generate_random_record(&rec, 1000);
            rec.id = key;
            printf("Add: ");
            print_record(&rec);
            btree_insert(pages_filename, records_filename, tree, &rec);
            sum += get_disk_reads();
            sumw += get_disk_writes();
            cnt++;
        }
        else if(command == 'o')
        {
            struct record rec;
            int id;
            int x;
            int a[5];
            fscanf(file, "%d %d %d %d %d %d %d", &id, &a[0], &a[1], &a[2], &a[3], &a[4], &x);
            record_set(&rec, id, a[0], a[1], a[2], a[3], a[4], x);
            printf("Add: ");
            print_record(&rec);
            btree_insert(pages_filename, records_filename, tree, &rec);
            sum += get_disk_reads();
            sumw += get_disk_writes();
            cnt++;
        }
        else if(command == 'p')
        {
            print_page_file(pages_filename);
        }
        else if(command == 'r')
        {
            print_record_file(records_filename);
        }
        else if(command == 't')
        {
            traverse_tree(pages_filename, records_filename, tree);
        }
        else if(command == 's')
        {
            set_disk_reads(0);
            int data_address;
            int key;
            fscanf(file, "%d", &key);
            if(btree_search(pages_filename, tree, key, &data_address))
            {
                printf("Record found: \n");
                struct record rec;
                record_read(records_filename, &rec, data_address);
                print_record(&rec);
            }
            else
                printf("Record with key %d was not found\n", key);
            sum += get_disk_reads();
            sumw += get_disk_writes();
            cnt++;
            printf("Operation done with %d disk reads.\n", get_disk_reads());
        }
    }
    
    float avg = ((float)sum)/cnt;
    float avgw = ((float)sumw)/cnt;
    printf("Average rpo: %f\nAverage wpo: %f\n", avg, avgw);
    fclose(file);
}

void interactive_mode(const char* pages_filename, const char* records_filename, struct btree *tree)
{
    char command;
    int sum = 0;
    int sumw = 0;
    int cnt = 0;

    while(scanf("%c", &command) != EOF)
    {
        if(command == 'a')
        {
            struct record rec;
            int key;
            scanf("%d", &key);
            generate_random_record(&rec, 1000);
            rec.id = key;
            printf("Add: ");
            print_record(&rec);
            btree_insert(pages_filename, records_filename, tree, &rec);
            sum += get_disk_reads();
            cnt++;
        }
        else if(command == 'o')
        {
            struct record rec;
            int id;
            int x;
            int a[5];
            scanf("%d %d %d %d %d %d %d", &id, &a[0], &a[1], &a[2], &a[3], &a[4], &x);
            record_set(&rec, id, a[0], a[1], a[2], a[3], a[4], x);
            printf("Add: ");
            print_record(&rec);
            btree_insert(pages_filename, records_filename, tree, &rec);
            sum += get_disk_reads();
            cnt++;
        }
        else if(command == 'p')
        {
            print_page_file(pages_filename);
        }
        else if(command == 'r')
        {
            print_record_file(records_filename);
        }
        else if(command == 't')
        {
            traverse_tree(pages_filename, records_filename, tree);
        }
        else if(command == 's')
        {
            set_disk_reads(0);
            int data_address;
            int key;
            scanf("%d", &key);
            if(btree_search(pages_filename, tree, key, &data_address))
            {
                printf("Record found: \n");
                struct record rec;
                record_read(records_filename, &rec, data_address);
                print_record(&rec);
            }
            else
                printf("Record with key %d was not found\n", key);
            sum += get_disk_reads();
            cnt++;
            printf("Operation done with %d disk reads.\n", get_disk_reads());
        }
    }

    float avg = ((float)sum)/cnt;
    printf("Average rpo: %f\n", avg);
}

int main(int argc, char **argv)
{
    const char* pages_filename = "./data/pages";
    const char* records_filename = "./data/records";

    bool file_mode = false;
    int order = 2;

    for(int i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "-f") == 0)
            file_mode = true;

        else if(strcmp(argv[i], "-o") == 0)
        {
            if(i + 1 == argc)
            {
                printf("Specify tree order\n");
                return 1;
            }
            order = atoi(argv[i + 1]);
            printf("Initialized tree with order %d\n", order);
        }
    }

    struct btree tree;
    tree.order = order;

    btree_init(pages_filename, records_filename, order, &tree);

    if(!file_mode)
        interactive_mode(pages_filename, records_filename, &tree); 
    else text_mode(pages_filename, records_filename, &tree);

    return 0;
}