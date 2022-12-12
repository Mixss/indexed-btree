#include "stack.h"

void stack_push(struct stack *s, int val)
{
    s->tab[s->size] = val;
    s->size = s->size + 1;
}

int stack_pop(struct stack *s)
{
    if(s->size == 0) return -1;
    s->size = s->size - 1;
    return s->tab[s->size];
}

int stack_len(struct stack *s)
{
    return s->size;
}

void stack_clear(struct stack *s)
{
    s->size = 0;
}
