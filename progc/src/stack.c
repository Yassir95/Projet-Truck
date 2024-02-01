#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

stack_t *stack_create() {
    stack_t *stack = malloc(sizeof(stack_t));

    if (stack == NULL) {
        fprintf(stderr, "error: malloc failed in stack_create\n");
        exit(EXIT_FAILURE);
    }

    stack->start = NULL;
    stack->end = NULL;
    stack->size = 0;

    return stack;
}

stack_node_t *stack_node_create(node_t *data) {
    stack_node_t *node = malloc(sizeof(stack_node_t));

    if (node == NULL) {
        fprintf(stderr, "error: malloc failed in stack_node_create\n");
        exit(EXIT_FAILURE);
    }

    node->data = data;
    node->next = NULL;

    return node;
}

void stack_push(stack_t *stack, node_t *data) {
    if (stack == NULL) {
        fprintf(stderr, "error: stack is NULL in stack_push\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *node = stack_node_create(data);

    if (stack->start == NULL) {
        stack->start = node;
        stack->end = node;
        stack->size = 1;
    } else {
        stack->end->next = node;
        stack->end = node;
        stack->size++;
    }
}

node_t *stack_pop(stack_t *stack) {
    if (stack == NULL) {
        fprintf(stderr, "error: stack is NULL in stack_pop\n");
        exit(EXIT_FAILURE);
    }

    if (stack->start == NULL) {
        fprintf(stderr, "error: stack is empty in stack_pop\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *first = stack->start;
    node_t *data = first->data;

    if (stack->start == stack->end) {
        stack->start = NULL;
        stack->end = NULL;
        stack->size = 0;
    } else {
        stack->start = stack->start->next;
        stack->size--;
    }

    free(first);

    return data;
}

void stack_print(stack_t *stack) {
    if (stack == NULL) {
        fprintf(stderr, "error: stack is NULL in stack_print\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *current = stack->start;

    printf("stack: %p\n", stack->start);

    while (current != NULL) {
        printf("-> %p ", current);
        current = current->next; 
    }
}

void stack_destroy(stack_t *stack) {
    if (stack == NULL) {
        fprintf(stderr, "error: stack is NULL in stack_destroy\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *current = stack->start;

    while (current != NULL) {
        stack_node_t *next = current->next;
        free(current);
        current = next;
    }

    free(stack);
}