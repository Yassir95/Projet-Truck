#ifndef AVL_H
#define AVL_H

enum data_tree_type_e {
    T,
    S
};

typedef enum data_tree_type_e data_tree_type_e;

struct data_tree_t
{
    char *key;
    unsigned int count;
    unsigned int start;
};

typedef struct data_tree_t data_tree_t;

struct data_tree_s
{
    unsigned int key;
    double minimum;
    double maximum;
    double distance;
    unsigned int count;
};

typedef struct data_tree_s data_tree_s;

union data_u {
    data_tree_t t;
    data_tree_s s;
};

typedef union data_u data_u;

struct node_t {
    struct node_t *left;
    struct node_t *right;
    int height;
    struct node_t *parent;
    data_u data;
};

typedef struct node_t node_t;

struct tree_t
{
    node_t *root;
    data_tree_type_e type;
};

typedef struct tree_t tree_t;


tree_t *create_avl(data_tree_type_e type);
node_t *find_avl(data_u value, tree_t *root);
void print_avl(tree_t *root);
void insert_avl(tree_t *root, data_u value);
void destroy_avl(tree_t *root);
#endif