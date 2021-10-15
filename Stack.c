// Copyright Raduta Lavinia && Baiatu Bianca 313CA 2021

#include <stdlib.h>
#include <stdio.h>

#include "Stack.h"
#include "utils.h"

stack_t *
st_create(unsigned int data_size)
{
	stack_t *st = (stack_t *)malloc(sizeof(stack_t));
	DIE(!st, "malloc failed!");
	st->list = ll_create(data_size);
	return st;
}

unsigned int
st_get_size(stack_t *st)
{
	if (st->list->head) {
		return ll_get_size(st->list);
	} else {
		return 0;
	}
}

unsigned int
st_is_empty(stack_t *st)
{
	if (st_get_size(st)) {
		return 0;
	} else {
		return 1;
	}
}

void *
st_peek(stack_t *st)
{
	if (st_get_size(st)) {
		return st->list->head->data;
	} else {
		return NULL;
	}
}

void
st_pop(stack_t *st)
{
	if (!st_is_empty(st)) {
		ll_node_t *popped = ll_remove_nth_node(st->list, 0);
		free(popped->data);
		free(popped);
	} else {
		printf("The stack is empty!");
	}
}

void
st_push(stack_t *st, void *new_data)
{
	ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t *st)
{
	while (!st_is_empty(st)) {
		st_pop(st);
	}
}

void
st_free(stack_t *st)
{
	st_clear(st);
	ll_free(&st->list);
	free(st);
}
