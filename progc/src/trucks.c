#include "trucks.h"
#include "avl.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


data_t *create_data(unsigned int road_id, unsigned int step_id, char *town_a, char *town_b, double distance) {
    data_t *data = malloc(sizeof(data_t));

    if (data == NULL) {
        fprintf(stderr, "Error: malloc failed in create_data\n");
        exit(EXIT_FAILURE);
    }
    
    data->road_id = road_id;
    data->step_id = step_id;
    data->town_a = town_a;
    data->town_b = town_b;
    data->distance = distance;

    return data;
}

void print_data(data_t *data) {
    if (data == NULL) {
        fprintf(stderr, "Error: data is NULL in print_data\n");
        exit(EXIT_FAILURE);
    }

    printf("road_id: %u\n", data->road_id);
    printf("step_id: %u\n", data->step_id);
    printf("town_a: %s\n", data->town_a);
    printf("town_b: %s\n", data->town_b);
    printf("distance: %f\n", data->distance);
}

void free_data(data_t *data) {
    if (data == NULL) {
        fprintf(stderr, "error: data is NULL in free_data\n");
        exit(EXIT_FAILURE);
    }

    free(data->town_a);
    free(data->town_b);
    free(data);
}

data_t *read_data(char *csv_line) {
    if (csv_line == NULL) {
        fprintf(stderr, "error: csv_line is NULL in read_data\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(csv_line, ";");
    unsigned int road_id = atoi(token);

    token = strtok(NULL, ";");
    unsigned int step_id = atoi(token);

    token = strtok(NULL, ";");
    char *town_a = malloc(strlen(token) + 1);
    strcpy(town_a, token);

    token = strtok(NULL, ";");
    char *town_b = malloc(strlen(token) + 1);
    strcpy(town_b, token);

    token = strtok(NULL, ";");
    double distance = atof(token);

    return create_data(road_id, step_id, town_a, town_b, distance);
}

void exports_to_csv(char *target_file, tree_t *tree) {
    FILE *fp = fopen(target_file, "w");

    if (fp == NULL) {
        fprintf(stderr, "error: fopen failed in exports_to_csv\n");
        exit(EXIT_FAILURE);
    }

    stack_t *stack = stack_create();
    stack_push(stack, tree->root);

    while (stack->size > 0) {
        node_t *node = stack_pop(stack);

        if (node->left != NULL) {
            stack_push(stack, node->left);
        }

        if (node->right != NULL) {
            stack_push(stack, node->right);
        }

        if (tree->type == S) {
            fprintf(fp, "%u;%f;%f;%f\n", node->data.s.key, node->data.s.distance/node->data.s.count, node->data.s.minimum, node->data.s.maximum);
        } else {
            fprintf(fp, "%s;%u;%u\n", node->data.t.key, node->data.t.count, node->data.t.start);
        }
    }

    stack_destroy(stack);

    if (fclose(fp) == EOF) {
        fprintf(stderr, "error: fclose failed in exports_to_csv\n");
        exit(EXIT_FAILURE);
    }
}