#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 128

struct stack
{
    int begin;
    int size;
    int tab[MAX_STACK_SIZE];
};

void stack_push(struct stack *s, int val);
int stack_pop(struct stack *s);
int stack_len(struct stack *s);
void stack_clear(struct stack *s);
int stack_pop_base(struct stack *s);

#endif