#ifndef TRUCKS_H
#define TRUCKS_H

#include "avl.h"

struct data
{
    unsigned int road_id;
    unsigned int step_id;
    char *town_a;
    char *town_b;
    double distance;
};

typedef struct data data_t;

data_t *create_data(unsigned int road_id, unsigned int step_id, char *town_a, char *town_b, double distance);
void print_data(data_t *data);
void free_data(data_t *data);
data_t *read_data(char *csv_line);
void exports_to_csv(char *target_file, tree_t *tree);

#endif