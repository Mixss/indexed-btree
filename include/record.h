#ifndef RECORD_H
#define RECORD_H
#include <stdio.h>
#include <stdbool.h>

/*44. File records: parameters a0,a1,a2,a3,a4,x.
Sorting by the value of the function g(x)=a0+a1x +a2x2+a3x3+a4x4*/
struct record 
{
    int id;
    int a[5];
    int x;
} __attribute__((packed)); 

/*fills record with zeros*/
void zero_record(struct record* rec);

void print_record(struct record* rec);

bool is_null(struct record *rec);

#endif