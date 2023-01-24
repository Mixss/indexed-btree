# B-tree as the index file

This project is the implementation of the B-tree structure in C. The data (records) stored in the B-tree are the parameters of the polynomial function and the abscissa in the form:

$g(x) = a_0 + a_1 * x + a_2 * x^2 + a_3 * x^3+ a_4 * x^4$ 

## Definition

The definition of the B-tree may vary in the literature. Here's how it is defined in this project:

Let $h>=0$, $d>=1$. B-tree with height $h$ and order $d$ is an ordered tree that is either empty ($h = 0$), or satisfies the following conditions:
- all leaves are at the same level equal to $h$,
- each page contains at most $2d$ keys,
- each page except the root page contains at least $d$ keys,
- if a non-leaf page has $m$ keys, then it has $m+1$ child pages.

Records are compared using the *key* field.

## Indexing the records

This B-tree uses the indexed file to store the records. This means that there are two files being used:
- **records file** - the file containing only the data of the records with their *keys*. Uses paging, so when the record is fetched, the whole page is loaded (one page contains four records).
- **indexed file** - the file that stores the structure of the B-tree, with the addresses to the records from the records file

## Usage

The program allows to define how it behaves by using program arguments:
- **-o** ***\<d\>*** - specifies the order of the tree; by default the order is set to 2,
- **-f** - programs loads all the instructions from the text file *input.txt*, then terminates.

By default program stars in the interactive mode. Available commands (to use in the text file mode as well):
- **a** ***\<key\>*** - adds the new record to the tree with random parameters and *key* key,
- **o** ***\<key\> \<a0\> \<a1\> \<a2\> \<a3\> \<a4\> \<x\>*** - adds the new record with *key* key and defined parameters,
- **s** ***\<key\>*** - searches the record by the defined key; if found prints all its parameters,
- **t** - traverses the tree from the smallest to the biggest key printing every record,
- **p** - prints the contents of the index file,
- **r** - prints the contents of the records file.

The number of disk accesses is printed after each operation. When the *End of File* symbol is detected, the program prints the average number of writes and reads to the disk.

## Compilation

This program can be compiled using `gcc` and `make`