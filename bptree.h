/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree header
*/

typedef int DATA;
typedef int KEY;

typedef struct leaf {
    KEY key;
    DATA data;

    struct leaf *next
} LEAF;

typedef struct node {
    NODE *parent;

    KEY *keys;
    NODE *children;

    LEAF *leaf;
} NODE;


bool compare_key(KEY a, Key b);

void set_order(int order);
DATA get_data(KEY key);
void insert_data(KEY key, DATA data);
void delete_data(KEY key);
void show_tree();
