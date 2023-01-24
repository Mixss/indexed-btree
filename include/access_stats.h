#ifndef ACCESS_STATS_H
#define ACCESS_STATS_H

static int disk_reads = 0;
static int disk_writes = 0;

int get_disk_reads();
int get_disk_writes();

void add_disk_read(int n);
void add_disk_write(int n);

void set_disk_reads(int n);
void set_disk_writes(int n);


#endif
