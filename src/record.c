#include "record.h"

/*44. File records: parameters a0,a1,a2,a3,a4,x.
Sorting by the value of the function g(x)=a0+a1x +a2x2+a3x3+a4x4*/
int get_record_sorting_value(struct record rec)
{
    int x = rec.x;
    return rec.a[0]  + rec.a[1] * x + rec.a[2] * x * x + rec.a[3] * x * x * x + rec.a[4] * x * x * x * x;
}

void zero_record(struct record* rec)
{
    (*rec).id = 0;
    (*rec).a[0] = 0;
    (*rec).a[1] = 0;
    (*rec).a[2] = 0;
    (*rec).a[3] = 0;
    (*rec).a[4] = 0;
    (*rec).x = 0;
}

void print_record(struct record* rec)
{
    printf("id=%d ->\t g(x) = %d\t\t(a0=%d, a1=%d, a2=%d, a3=%d, a4=%d, x=%d)\n", (*rec).id, get_record_sorting_value((*rec)),  (*rec).a[0], (*rec).a[1], (*rec).a[2], (*rec).a[3], (*rec).a[4], (*rec).x);
}

bool is_null(struct record *rec)
{
    return (rec->id == 0 && rec->a[0] == 0 && rec->a[1] == 0 && rec->a[2] == 0 && rec->a[3] == 0 && rec->a[04] == 0 && rec->x == 0);
}

void record_set_with_array(struct record *rec, int id, int a[5], int x)
{
    rec->id = id;
    rec->a[0] = a[0];
    rec->a[1] = a[1];
    rec->a[2] = a[2];
    rec->a[3] = a[3];
    rec->a[4] = a[4];
    rec->x = x;
}

void record_set(struct record *rec, int id, int a0, int a1, int a2, int a3, int a4, int x)
{
    rec->id = id;
    rec->a[0] = a0;
    rec->a[1] = a1;
    rec->a[2] = a2;
    rec->a[3] = a3;
    rec->a[4] = a4;
    rec->x = x;
}

void generate_random_record(struct record* rec, int max_value)
{
    (*rec).a[0] = (rand() % (2 * max_value)) - max_value;
    (*rec).a[1] = (rand() % (2 * max_value)) - max_value;
    (*rec).a[2] = (rand() % (2 * max_value)) - max_value;
    (*rec).a[3] = (rand() % (2 * max_value)) - max_value;
    (*rec).a[4] = (rand() % (2 * max_value)) - max_value;
    (*rec).x = (rand() % (2 * max_value)) - max_value;
}