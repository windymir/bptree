/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree
*/

#include <stdio.h>
#include "bptree.h"

int order = 2;
NODE *root = NULL;


bool compare_key(KEY a, Key b){
    // KEY 타입 변경시 같이 수정 필요함
    return a < b;
}

bool is_key_equals(KEY a, KEY b) {
    // KEY 타입 변경시 같이 수정 필요함
    return a == b;
}


NODE *get_children(NODE *parent, KEY key) {
    NODE p = *parent;

    if (p.children == NULL)
        return NULL;

    for (int i = 0; i < order; i++) {
        if (compare_key(key, p.keys[i]))
            return p.children[i];
    }
    return p.children[order];
}

NODE *get_leaf(NODE *parent, KEY key) {
    if (parent == NULL)
        return NULL;

    NODE *children = get_children(parent, key);
    if (children != NULL) {
        return get_leaf(children, key);
    } else {
        return parent;
    }
}

void set_order(int ord) {
    order = ord;
}

DATA get_data(KEY key) {
    NODE *leaf = get_leaf(root, key);
    if (leaf == NULL) {
        #ifdef
        printf("Cannot find leaf node");
        #endif
        return NULL;
    } else {
        for (int i = 0; i < order; i++) {
            if (is_key_equals(key, (*leaf).keys[i]))
                return (*leaf).data[i];
        }
        printf("Cannot find key from b+ tree");
        return NULL;
    }
}

NODE *get_next_leaf(NODE *leaf) {
    // FIXME: Implement
}

void insert_data(KEY key, DATA data){
    // FIXME: Implement
}

void delete_data(KEY key) {
    // FIXME: Implement
}

void show_tree() {
    // FIXME: Implement
}
