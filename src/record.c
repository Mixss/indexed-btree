#include "record.h"

void zero_record(struct record* rec)
{
    (*rec).id = -1;
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
    return rec->id == -1 ? true : false;
}