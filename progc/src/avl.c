#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl.h"
#include "stack.h"

tree_t *create_avl(data_tree_type_e type)
{
    tree_t *tree = malloc(sizeof(tree_t));

    if (tree == NULL)
    {
        fprintf(stderr, "error: malloc failed in create_avl\n");
        exit(EXIT_FAILURE);
    }

    tree->type = type;
    tree->root = NULL;

    return tree;
}

node_t *create_node_avl(data_u value)
{
    node_t *node = malloc(sizeof(node_t));

    if (node == NULL)
    {
        fprintf(stderr, "error: malloc failed in create_node_avl\n");
        exit(EXIT_FAILURE);
    }

    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->height = 0;

    return node;
}

node_t *compare_avl(node_t *parent, data_tree_type_e type, data_u *value)
{
    if (parent == NULL)
    {
        fprintf(stderr, "error: parent is NULL in compare_avl\n");
        exit(EXIT_FAILURE);
    }

    if (type == S)
    {
        if (parent->data.s.key > value->s.key)
        {
            return parent->left;
        }
        else if(parent->data.s.key < value->s.key)
        {
            return parent->right;
        }
    }
    else
    {
        int cmp = strcmp(parent->data.t.key, value->t.key);
        if (cmp > 0)
        {
            return parent->left;
        }
        else if (cmp < 0)
        {
            return parent->right;
        }
    }

    return parent;
}

node_t *find_avl(data_u value, tree_t *tree)
{
    node_t *current = tree->root;

    while (current != NULL)
    {
        node_t *next = compare_avl(current, tree->type, &value);

        if (current == next)
        {
            return current;
        }

        current = next;
    }

    return NULL;
}

void print_node_avl(node_t *node, data_tree_type_e type, int level) {
    if (node == NULL)
    {
        return;
    }

    print_node_avl(node->right, type, level + 1);

    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    if (type == S)
    {
        printf("%d (%d)\n", node->data.s.key, node->parent != NULL ? node->parent->data.s.key : 0);
    }
    else
    {
        printf("%s (%s)\n", node->data.t.key, node->parent != NULL ? node->parent->data.t.key : "NONE");
    }

    print_node_avl(node->left, type, level + 1);
}

void print_avl(tree_t *tree)
{  
    print_node_avl(tree->root, tree->type, 0);
}

int height(node_t *node)
{
    if (node == NULL)
        return -1;

    return node->height;
}

int max(int a, int b)
{
    return (a >= b) ? a : b;
}

node_t *right_rotate(node_t *root)
{
    node_t *pivot = root->left;

    root->left = pivot->right;
    if (pivot->left != NULL)
        pivot->left->parent = pivot;
    pivot->right = root;
    pivot->parent = root->parent;
    root->parent = pivot;

    root->height = max(height(root->left),
                    height(root->right)) +
                1;
    pivot->height = max(height(pivot->left),
                        height(pivot->right)) +
                    1;

    if (pivot->right->right != NULL) {
        pivot->right->right->parent = pivot->right;
    }

    if (pivot->right->left != NULL) {
        pivot->right->left->parent = pivot->right;
    }

    return pivot;
}

node_t *left_rotate(node_t *root)
{
    node_t *pivot = root->right;

    // Perform rotation
    root->right = pivot->left;
    if (pivot->right != NULL)
        pivot->right->parent = pivot;
    pivot->left = root;

    pivot->parent = root->parent;
    root->parent = pivot;

    // Update heights
    root->height = max(height(root->left),
                    height(root->right)) +
                1;
    pivot->height = max(height(pivot->left),
                    height(pivot->right)) +
                1;

    if (pivot->left->right != NULL) {
        pivot->left->right->parent = pivot->left;
    }

    if (pivot->left->left != NULL) {
        pivot->left->left->parent = pivot->left;
    }

    // Return new root
    return pivot;
}

node_t *rotate(node_t *node)
{
    int balance_factor = height(node->left) - height(node->right);

    // Left Left Case
    if (balance_factor > 1)
    {
        int right_balance_factor = height(node->left->left) - height(node->left->right);

        if (right_balance_factor < 0)
        {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
        else
        {
            return right_rotate(node);
        }
    }
    else if (balance_factor < -1)
    {
        int left_balance_factor = height(node->right->left) - height(node->right->right);

        if (left_balance_factor > 0)
        {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
        else
        {
            return left_rotate(node);
        }
    }

    return node;
}

void insert_avl(tree_t *tree, data_u value)
{
    node_t *current = tree->root;
    node_t *parent = NULL;

    while (current != NULL)
    {
        parent = current;

        if (tree->type == S)
        {
            if (current->data.s.key > value.s.key)
            {
                current = current->left;
            }
            else if (current->data.s.key < value.s.key)
            {
                current = current->right;
            }
            else
            {
                current->data.s.count = current->data.s.count + 1;
                current->data.s.distance += value.s.distance;
                if (current->data.s.minimum > value.s.minimum)
                {
                    current->data.s.minimum = value.s.minimum;
                }

                if (current->data.s.maximum < value.s.maximum)
                {
                    current->data.s.maximum = value.s.maximum;
                }
                return;
            }
        }
        else
        {
            int cmp = strcmp(current->data.t.key, value.t.key);

            if (cmp > 0)
            {
                current = current->left;
            }
            else if (cmp < 0)
            {
                current = current->right;
            }
            else
            {
                current->data.t.count = current->data.t.count + value.t.count;
                current->data.t.start = current->data.t.start + value.t.start;
                return;
            }
        }
    }

    node_t *new_node = create_node_avl(value);

    if (parent == NULL)
    {
        tree->root = new_node;
        return;
    }

    if (tree->type == S)
    {
        if (parent->data.s.key > value.s.key)
        {
            parent->left = new_node;
        }
        else
        {
            parent->right = new_node;
        }
    }
    else
    {
        if (strcmp(parent->data.t.key, value.t.key) > 0)
        {
            parent->left = new_node;
        }
        else
        {
            parent->right = new_node;
        }

    }

    new_node->parent = parent;
    current = new_node;

    while (current != NULL)
    {
        // update height
        int left_height = height(current->left);
        int right_height = height(current->right);

        current->height = 1 + max(left_height, right_height);

        node_t *rotated = rotate(current);

        if (current->parent == NULL) {
            tree->root = rotated;
        } else {            
            if (rotated->parent != NULL) {
                if (rotated->parent->left == current) {
                    rotated->parent->left = rotated;
                } else {
                    rotated->parent->right = rotated;
                }
            }
        }

        current = current->parent;
    }
}

void destroy_node_avl(node_t *node, data_tree_type_e type)
{
    if (node == NULL)
    {
        return;
    }

    stack_t *stack = stack_create();
    stack_push(stack, node);

    while (stack->size > 0)
    {
        node_t *current = stack_pop(stack);

        if (current->left != NULL)
        {
            stack_push(stack, current->left);
        }

        if (current->right != NULL)
        {
            stack_push(stack, current->right);
        }

        if (type == T)
        {
            free(current->data.t.key);
        }

        free(current);
    }

    stack_destroy(stack);
}

void destroy_avl(tree_t *root) {
    if (root == NULL) {
        fprintf(stderr, "error: root is NULL in destroy_avl\n");
        exit(EXIT_FAILURE);
    }

    destroy_node_avl(root->root, root->type);
    free(root);
}