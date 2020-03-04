/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree
*/

#include "bptree.h"


int maximum_key = 2;
int minimum_key = 1;
int minimum_children = 2;
NODE *root = NULL;


void set_maximum_key(int order) {
    if (order < 2) {
        printf("Input maximum key larger than 1\n");
        printf("Use default maximum key %d\n", maximum_key);
        fflush(stdout);
        return;
    }

    maximum_key = order;
    minimum_key = (order + 1) / 2;
    minimum_children = minimum_key + 1;
}

DATA *get_data(KEY key) {
    NODE *leaf = get_leaf(root, key);
    int idx = get_key_idx(leaf, key);
    if (idx == -1) {
#ifdef DEBUG
        printf("Cannot find key from b+ tree\n");
        fflush(stdout);
#endif
        return NULL;
    } else {
        return leaf->keys[idx].data;
    }
}

void insert_data(int key, DATA data) {
    // KEY, DATA 변경에 따라 수정 필요함
    if (root == NULL) {
        root = create_node(maximum_key);
    }

    KEY key_data = create_key_data(key, data);
    if (get_data(key_data) != NULL) {
        printf("Key %d already exists.\n", key);
        return;
    }

    NODE *leaf = get_leaf(root, key_data);
    insert_key_tree(leaf, key_data, NULL);
}

void delete_data(int key_value) {
    if (root == NULL) {
        printf("Empty tree");
        return;
    }
    
    printf("Delete key %d\n", key_value);
    KEY key = create_key(key_value);
    NODE *leaf = get_leaf(root, key);
    delete_key_tree(leaf, key);
    printf("Delete key %d complete\n", key_value);
}

void range_search(int min, int max) {
    // 입력받은 키값을 포함하여 검색
    KEY from = create_key(min);
    KEY to = create_key(max);
    NODE *leaf1 = get_leaf(root, from);
    NODE *leaf2 = get_leaf(root, to);

    printf("Range Search result: %d ~ %d\n", min, max);

    NODE *cursor = leaf1;
    if (cursor == NULL || min > max) {
        printf("Cannot find result.\n");
        fflush(stdout);
        return;
    }

    printf(" | ");
    do {
        int comma = 0;
        for(int i = 0; i < cursor->key_count; i++) {
            KEY key = cursor->keys[i];

            if ((compare_key(from, key) || is_key_equals(from, key)) 
                    && (compare_key(key, to) || is_key_equals(to, key))) {
                if (comma)
                    printf(", ");
                printf("%d (Data: %d)", key.key, *(key.data));
                comma = 1;
            } else {
                comma = 0;
            }
        }
        printf(" | ");
        cursor = cursor->next_leaf;
    } while(cursor != leaf2->next_leaf);

    printf("\nSearch result end\n");
    fflush(stdout);
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
        printf("%d children in node\n", node->children_count);
        fflush(stdout);

        for (int i = 0; i < node->children_count; i++) {
            show_tree(node->children[i]);
        }
    } else {
        printf("==============TREE_BRANCH_END===============\n");
    }
}
