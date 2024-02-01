#ifndef STACK_H
#define STACK_H

#include "avl.h"

struct stack_node_t {
    node_t *data;
    struct stack_node_t *next;
};

typedef struct stack_node_t stack_node_t;

typedef struct stack_t {
    stack_node_t *start;
    stack_node_t *end;
    int size;
} stack_t;

stack_t *stack_create();
void stack_push(stack_t *stack, node_t *data);
node_t *stack_pop(stack_t *stack);
void stack_print(stack_t *stack);
void stack_destroy(stack_t *stack);

#endif