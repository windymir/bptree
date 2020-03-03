/*
    Author: Soeun Kim
    Date: 3, Mar, 2020
    Description: B+ tree node manager
*/

#include "node.h"


extern int maximum_key;
extern int minimum_key;
extern NODE *root;


NODE *create_node() {
    NODE *np = (NODE*)malloc(sizeof(NODE));
    if (np == NULL) {
        printf("Node initialize failed.");
        fflush(stdout);
        exit(1);
        return NULL;
    } else {
        np->key_count = 0;
        np->parent = NULL;
        // Node 분리시 편하려고 key 메모리 1칸 더 추가
        np->keys = (KEY*) malloc((maximum_key + 1) * sizeof(KEY));
        np->children = NULL;
        np->next_leaf = NULL;
    }
    return np;
}

NODE **create_children() {
    // Childeren node array 크기는 최대 키 갯수 + 1 에 split용 여유분 1칸 추가
    NODE **children = (NODE **)malloc(sizeof(NODE*) * (maximum_key + 2));
    for (int i = 0; i < maximum_key + 2; i++) {
        children[i] = (NODE *)malloc(sizeof(NODE));
        children[i] = NULL;
    }

    return children;
}

NODE **splice_node(NODE ***array, int length, int splice_start_idx, int splice_end_idx) {
    NODE **nodes = *array;

    int splice_length = splice_end_idx - splice_start_idx;
    if (splice_end_idx > length + 1 || splice_length <= 0)
        // Cannot splice nodes
        return NULL;

    NODE **spliced_nodes = create_children();
    for (int i = splice_start_idx; i < splice_end_idx; i++) {
        spliced_nodes[i - splice_start_idx] = nodes[i];
    }

    for (int i = splice_start_idx; i < length - splice_length; i++) {
        nodes[i] = nodes[i + splice_length];
    }

    return spliced_nodes;
}

void split_full_node(NODE *np, NODE **new_child, KEY *parent_key) {
#ifdef DEBUG
    printf("split_full_node ");
    for (int i = 0; i < np->key_count; i++)
        printf("%d ", np->keys[i].key);
    printf("\n");
    fflush(stdout);
#endif

    // 임시로 max key보다 1개 많은 key를 가진 node를 분리
    // 기존 node에 minimum key 갯수만큼 남기고 새로운 node로 이동
    KEY *spliced_keys = splice_key(&(np->keys), np->key_count, minimum_key, np->key_count, 1);

    int children_count = np->key_count + 1;
    int new_key_count = np->key_count - minimum_key;

    np->key_count = minimum_key;

    NODE *new_node = create_node();
    // Move spliced keys to new node
    for (int i = 0; i < new_key_count; i++) {
        if (i == 0 && np->children != NULL)
            continue;

        new_node->keys[new_node->key_count].key = spliced_keys[i].key;
        new_node->keys[new_node->key_count].data = NULL;
        if (np->children == NULL)
            new_node->keys[new_node->key_count].data = spliced_keys[i].data;
        new_node->key_count++;
    }

    if (np->children == NULL) {
        // link leaf nodes
        new_node->next_leaf = np->next_leaf;
        np->next_leaf = new_node;
    } else {
        // Move spliced children to new node
        // 기존 node에 minimum key + 1 갯수만큼 남기고 새로운 node로 이동
        new_node->children = splice_node(&(np->children), children_count, minimum_key + 1, children_count + 1);
        // parent node 변경
        for (int j = 0; j < children_count - minimum_key - 1; j++)
            new_node->children[j]->parent = new_node;
    }

    parent_key->key = spliced_keys[0].key;
    parent_key->data = NULL;
    *new_child = new_node;

    free(spliced_keys);
}

NODE *get_child(NODE *parent, KEY key) {
    if (parent->children == NULL)
        return NULL;

    for (int i = 0; i < parent->key_count; i++) {
        if (compare_key(key, parent->keys[i]))
            return parent->children[i];
    }
    return parent->children[parent->key_count];
}

NODE *get_leaf(NODE *parent, KEY key) {
    if (parent == NULL) {
        printf("Create node first.");
        exit(1);
    }

    NODE *child = get_child(parent, key);
    if (child != NULL) {
        return get_leaf(child, key);
    } else {
        // 재귀 종료
        return parent;
    }
}

int get_insert_idx(NODE *np, KEY key) {
    int new_key_idx = np->key_count;
    for (int i = 0; i < np->key_count; i++) {
        if (compare_key(key, np->keys[i])) {
            new_key_idx = i;
            break;
        }
    }

    return new_key_idx;
}

int get_key_idx(NODE *np, KEY key) {
    for (int i = 0; i < np->key_count; i++) {
        if (is_key_equals(key, np->keys[i]))
            return i;
    }
    return -1;
}

void insert_node(NODE ***array, int length, int insert_idx, NODE *new_child) {
    NODE **children = *array;
    for (int i = length; i > insert_idx; i--) {
        children[i] = children[i - 1];
    }
    children[insert_idx] = new_child;
}

int insert_key_current_node(NODE *np, KEY key, NODE *new_child) {
    int new_key_idx = get_insert_idx(np, key);
    insert_key(&(np->keys), np->key_count, new_key_idx, &key);
    np->key_count++;

    if (np->children != NULL) {
        insert_node(&(np->children), np->key_count, new_key_idx + 1, new_child);
    }

#ifdef DEBUG
    printf("insert_key_current_node idx %d\n", new_key_idx);
    fflush(stdout);
#endif
    return new_key_idx;
}

void insert_key_tree(NODE *np, KEY key, NODE *new_child) {
#ifdef DEBUG
    printf("insert_key_tree %d\n", key.key);
    fflush(stdout);
#endif

    insert_key_current_node(np, key, new_child);

    if(np->key_count > maximum_key) {
        NODE *child;
        KEY parent_key;
        split_full_node(np, &child, &parent_key);

        if (np->parent == NULL) {
            // Root node 변경
            np->parent = create_node();
            np->parent->children = create_children();
            np->parent->children[0] = np;
            root = np->parent;
        }
        child->parent = np->parent;
        insert_key_tree(np->parent, parent_key, child);
    } else {
        // 재귀 종료
        return;
    }
}
