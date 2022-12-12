#include "access_stats.h"

int get_disk_reads()
{
    return disk_reads;
}
int get_disk_writes()
{
    return disk_writes;
}

void add_disk_read(int n)
{
    disk_reads += n;
}
void add_disk_write(int n)
{
    disk_writes += n;
}

void set_disk_reads(int n)
{
    disk_reads = n;
}
void set_disk_writes(int n)
{
    disk_writes = n;
}
