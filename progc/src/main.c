#include "main.h"
#include "trucks.h"
#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>

struct progress_t {
    int total;
    FILE *fp;
};

typedef struct progress_t progress_t;

int display_progress(progress_t *data)
{  
    FILE *fp = ((progress_t *) data)->fp;
    int total = ((progress_t *) data)->total;
    int progress = ftell(fp);
    int last_progress = (int)(((float)progress / total) * 100);

    while (true)
    {
        progress = ftell(fp);
        int new_progress = (int)(((float)progress / total) * 100);

        if (new_progress > last_progress)
        {
            last_progress = new_progress;
            printf("\rprocessing: %d%%", new_progress);
            fflush(stdout);
        }

        if (progress == total)
            break;
    }

    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        fprintf(stderr, "error: invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    bool do_cities = atoi(argv[2]);
    bool do_routes = atoi(argv[3]);
    char *output_t = argv[4];
    char *output_s = argv[5];

    if (!do_routes && !do_cities)
        return EXIT_SUCCESS;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "error: fopen failed in main\n");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    size_t read;

    tree_t *tree_routes = NULL;
    tree_t *tree_cities = NULL;

    if (do_routes)
    {
        tree_routes = create_avl(S);
    }

    if (do_cities)
    {
        tree_cities = create_avl(T);
    }

    // Skip the first line
    getline(&line, &len, fp);
    fseek(fp, 0L, SEEK_END);
    int filesize = ftell(fp);
    rewind(fp);

    // launch a thread to display progress
    thrd_t progress_thread;
    progress_t *args = malloc(sizeof(progress_t));
    args->total = filesize;
    args->fp = fp;
    thrd_create(&progress_thread, (thrd_start_t)display_progress, args);

    while ((read = getline(&line, &len, fp)) != (long unsigned int)-1)
    {
        data_t *data = read_data(line);

        if (do_routes)
        {
            insert_avl(tree_routes, (data_u){
                                        .s = {
                                            .key = data->road_id,
                                            .minimum = data->distance,
                                            .maximum = data->distance,
                                            .distance = data->distance,
                                            .count = 1}});
        }

        if (do_cities)
        {
            insert_avl(tree_cities, (data_u){
                                        .t = {
                                            .key = data->town_a,
                                            .count = 1,
                                            .start = data->step_id == 1 ? 1 : 0}});

            insert_avl(tree_cities, (data_u){
                                        .t = {
                                            .key = data->town_b,
                                            .count = 1}});
        }

        free(data);
    }

    thrd_join(progress_thread, NULL);

    if (do_routes)
    {
        exports_to_csv(output_s, tree_routes);
        destroy_avl(tree_routes);
    }

    if (do_cities)
    {
        exports_to_csv(output_t, tree_cities);
        destroy_avl(tree_cities);
    }

    free(line);

    if (fclose(fp) == EOF)
    {
        fprintf(stderr, "error: fclose failed in main\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}