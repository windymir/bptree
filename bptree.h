/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree header
*/

typedef int DATA;
typedef int KEY;

typedef struct node {
    NODE *parent;

    KEY *keys;
    NODE *children;

    // For leaf node only
    DATA *data;
    struct node *next_leaf;
} NODE;


bool compare_key(KEY a, KEY b);
bool is_key_equals(KEY a, KEY b);
NODE *get_children(NODE *parent, KEY key);
NODE *get_leaf(NODE *parent, KEY key);

void set_order(int order);
DATA get_data(KEY key);
NODE *get_next_leaf(NODE *leaf);
void insert_data(KEY key, DATA data);
void delete_data(KEY key);
void show_tree();
