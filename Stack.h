#ifndef STACK_H_
#define STACK_H_

#include "LinkedList.h"

typedef struct stack_t stack_t;
struct stack_t
{
	struct linked_list_t *list;
};

stack_t *
st_create(unsigned int data_size);

unsigned int
st_get_size(stack_t *st);

unsigned int
st_is_empty(stack_t *st);

void *
st_peek(stack_t *st);

void
st_pop(stack_t *st);

void
st_push(stack_t *st, void *new_data);

void
st_clear(stack_t *st);

void
st_free(stack_t *st);

#endif  /* STACK_H_ */
