/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree header
*/
#include "key.h"

typedef struct node {
    NODE *parent;

    int key_count;
    KEY *keys;
    NODE *children;

    // For leaf node only
    struct node *prev_leaf;
    struct node *next_leaf;
} NODE;


void create_node(NODE **np);

NODE *get_children(NODE *parent, KEY key);
NODE *get_leaf(NODE *parent, KEY key);

void set_order(int order);
DATA get_data(KEY key);
void insert_data(KEY key, DATA data);
void delete_data(KEY key);
void range_search(KEY from, KEY to);
void show_tree();
