/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree header
*/
#include "key.h"

typedef struct node {
    struct node *parent;

    int key_count;
    KEY *keys;
    struct node **children;

    // For leaf node only
    struct node *next_leaf;
} NODE;


NODE *create_node();
NODE **create_children();

NODE *get_child(NODE *parent, KEY key);
NODE *get_leaf(NODE *parent, KEY key);
NODE **splice_node(NODE ***array, int length, int splice_start_idx, int splice_end_idx);
// max key보다 1개 많은 key를 가진 node 분리
void split_full_node(NODE *np, NODE **new_child, KEY *parent_key);
int get_insert_idx(NODE *np, KEY key);
void insert_node(NODE ***array, int length, int insert_idx, NODE *new_child);
int insert_key_current_node(NODE *np, KEY key, NODE *new_child);
void insert_key_tree(NODE *np, KEY key, NODE *new_child);

void set_maximum_key(int order);
DATA *get_data(KEY key);
void insert_data(KEY key_data);
void delete_data(KEY key);
void range_search(KEY from, KEY to);
void show_tree();
