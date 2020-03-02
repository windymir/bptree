/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree
*/

#include "bptree.h"


int maximum_key = 2;
int minimum_key = 1;
NODE *root = NULL;


void set_maximum_key(int order) {
    if (order < 2) {
        printf("Input maximum key larger than 1\n");
        fflush(stdout);
        return;
    }

    maximum_key = order;
    if (order % 2 == 0)
        minimum_key = order / 2;
    else
        minimum_key = (order + 1) / 2;
}

DATA *get_data(KEY key) {
    NODE *leaf = get_leaf(root, key);
    printf("Finding key: %d\n", key.key);
    for (int i = 0; i < leaf->key_count; i++) {
        printf("Current cursor: %d\n", leaf->keys[i].key);
        if (is_key_equals(key, leaf->keys[i]))
            return leaf->keys[i].data;
    }
    printf("Cannot find key from b+ tree\n");
    fflush(stdout);
    return NULL;
}

void insert_data(int key, DATA data) {
    // KEY, DATA 변경에 따라 수정 필요함
    if (root == NULL) {
        root = create_node(maximum_key);
    }

    KEY key_data = create_key_data(key, data);
    NODE *leaf = get_leaf(root, key_data);
    insert_key_tree(leaf, key_data, NULL);
}

void delete_data(KEY key) {
    // FIXME: Implement
}

void range_search(KEY from, KEY to) {
    // FIXME: Implement
}

void show_tree(NODE *node) {
    printf("==============NODE===============\n");
    printf("%d keys in node\n", node->key_count);
    fflush(stdout);
    for (int i = 0; i < node->key_count; i++) {
        printf("Key: %d\n", node->keys[i].key);
        if (node->keys[i].data != NULL)
            printf("Data: %d\n", *(node->keys[i].data));
    }
    fflush(stdout);

    if (node->children != NULL) {
        printf("%d children in node\n", node->key_count + 1);
        fflush(stdout);

        for (int i = 0; i < node->key_count + 1; i++) {
            show_tree(node->children[i]);
        }
    } else {
        printf("==============TREE_BRANCH_END===============\n");
    }
}
