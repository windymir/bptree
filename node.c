/*
    Author: Soeun Kim
    Date: 3, Mar, 2020
    Description: B+ tree node manager
*/

#include "node.h"


extern int maximum_key;
extern int minimum_key;
extern int minimum_children;
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
        np->children_count = 0;
        np->parent = NULL;
        // Node 분리시 편하려고 key 메모리 1칸 더 추가
        np->keys = (KEY*) malloc((maximum_key + 1) * sizeof(KEY));
        np->children = NULL;
        np->prev_leaf = NULL;
        np->next_leaf = NULL;
    }
    return np;
}

void free_node_mem(NODE *np, int free_data) {
    if (np->children != NULL)
        free(np->children);

    if (free_data) {
        for (int i = 0; i < np->key_count; i++) {
            if (np->keys[i].data != NULL)
                free(np->keys[i].data);
        }
    }

    free(np->keys);
    free(np);
}

NODE **create_children() {
    // Childeren node array 크기는 최대 키 갯수 + 1 에 split용 여유분 1칸 추가
    NODE **children = (NODE **)malloc(sizeof(NODE*) * (maximum_key + 2));
    for (int i = 0; i < maximum_key + 2; i++) {
        children[i] = NULL;
    }

    return children;
}

NODE **splice_node(NODE ***array, int length, int splice_start_idx, int splice_end_idx, int get_spliced_node) {
    NODE **nodes = *array;

    int splice_length = splice_end_idx - splice_start_idx;
    if (splice_end_idx > length + 1 || splice_length <= 0)
        // Cannot splice nodes
        return NULL;

    NODE **spliced_nodes = NULL;
    if (get_spliced_node) {
        spliced_nodes = create_children();
        for (int i = splice_start_idx; i < splice_end_idx; i++) {
            spliced_nodes[i - splice_start_idx] = nodes[i];
        }
    } else {
        for (int i = splice_start_idx; i < splice_end_idx; i++) {
            free_node_mem(nodes[i], 0);
        }
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
    // 절반을 기존 node에 남기고 새로운 node로 이동
    // 홀수일 때 새로운 node로 이동하는 갯수가 더 많음
    KEY *spliced_keys = splice_key(&(np->keys), np->key_count, np->key_count/2, np->key_count, 1);

    int new_key_count = np->key_count - np->key_count/2;

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
        new_node->prev_leaf = np;
    } else {
        // Move spliced children to new node
        // 기존 node에 남은 키 + 1 갯수만큼 남기고 새로운 node로 이동
        new_node->children = splice_node(&(np->children), np->children_count, np->key_count/2 + 1, np->children_count + 1, 1);
        new_node->children_count = new_node->key_count + 1;
        // parent node 변경
        for (int j = 0; j < new_node->children_count; j++)
            new_node->children[j]->parent = new_node;
    }

    // 기존 노드 count 변경
    np->key_count = np->key_count/2;
    np->children_count = np->key_count + 1;

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
    return parent->children[parent->children_count - 1];
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
    insert_key(&(np->keys), np->key_count, new_key_idx, key);
    np->key_count++;

    if (np->children != NULL && new_child != NULL) {
        insert_node(&(np->children), np->children_count, new_key_idx + 1, new_child);
        np->children_count++;
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
            np->parent->children_count = 1;
            root = np->parent;
        }
        child->parent = np->parent;
        insert_key_tree(np->parent, parent_key, child);
    } else {
        // 재귀 종료
        return;
    }
}

int get_current_node_idx(NODE *cursor) {
    // parent의 children array에서의 cursor node idx
    // 없는 경우 -1
    if (cursor->parent == NULL)
        return -1;

    NODE **siblings = cursor->parent->children;
    int sibling_count = cursor->parent->children_count;
    for (int i = 0; i < sibling_count; i++) {
        if (cursor == siblings[i]) {
            return i;
        }
    }
    return -1;
}

int delete_key_current_node(NODE *np, KEY key) {
    int key_idx = get_key_idx(np, key);
    if (key_idx < 0)
        return -1;
    splice_key(&(np->keys), np->key_count, key_idx, key_idx + 1, 0);
    np->key_count--;
#ifdef DEBUG
    printf("delete_key_current_node %d\n", key.key);
    fflush(stdout);
#endif
    return key_idx;
}

KEY get_smallest_key(NODE *np) {
    // 자손 중 최소 key
    if (np->children == NULL) {
        // 재귀 종료
#ifdef DEBUG
    printf("get_smallest_key\n", np->keys[0].key);
    fflush(stdout);
#endif
        return np->keys[0];
    } else {
        return get_smallest_key(np->children[0]);
    }
}

void merge_nodes(NODE *left, NODE *right, int parent_key_idx) {
#ifdef DEBUG
    printf("merge_nodes\n");
    fflush(stdout);
#endif
    int left_last_idx = left->key_count - 1;
    // Left에 right를 merge
    for (int i = 0; i < right->key_count; i++) {
        insert_key(&(left->keys), left->key_count, left->key_count, right->keys[i]);
        left->key_count++;
    }

    // Left children에 right children 이동
    for (int i = 0; i < right->children_count; i++) {
        insert_node(&(left->children), left->children_count, left->children_count, right->children[i]);
        left->children_count++;
    }

    // 기존 left와 right join지점 children값에 따라 key 수정
    if (left->children != NULL)
        left->keys[left_last_idx] = get_smallest_key(left->children[left_last_idx + 1]);

    // parent key 삭제
    splice_key(&(left->parent->keys), left->parent->key_count, parent_key_idx, parent_key_idx + 1, 0);
    left->parent->keys--;

    if (left->children == NULL) {
        // Link leaf nodes
        left->next_leaf = right->next_leaf;
        if (right->next_leaf != NULL)
            right->next_leaf->prev_leaf = left;
    }

    // right node 삭제
    int right_node_idx = parent_key_idx + 1;
    splice_node(&(left->parent->children), left->parent->children_count, right_node_idx, right_node_idx + 1, 0);
    left->parent->children_count--;
    free_node_mem(right, 0);
}

void redistribute_key(NODE *np) {
    if (np->parent == NULL)
        // root node가 leaf node일 때 redistribute 필요 없음
        return;
#ifdef DEBUG
    printf("redistribute key\n");
    fflush(stdout);
#endif

    // For Leaf node
    int current_idx = get_current_node_idx(np);
    NODE *left = NULL;
    NODE *right = NULL;
    if (current_idx > 0)
        left = np->parent->children[current_idx - 1];
    if (current_idx < np->parent->children_count - 1)
        right = np->parent->children[current_idx + 1];
        
    if (left != NULL && left->key_count > minimum_key) {
        // left sibling에게서 마지막 key 1개 가져오기
        KEY new_key = left->keys[left->key_count - 1];
        left->key_count--;

        // 가져온 key를 맨 앞에 추가
        insert_key(&(np->keys), np->key_count, 0, new_key);
        np->key_count++;
    } else if (right != NULL && right->key_count > minimum_key) {
        // right sibling에게서 가장 앞 key 1개 가져오기
        KEY *new_key = splice_key(&(right->keys), right->key_count, 0, 1, 1);
        right->key_count--;

        // right sibling parent key 수정
        np->parent->keys[current_idx] = create_key(right->keys[0].key);

        // 가져온 key를 맨 뒤에 추가
        insert_key(&(np->keys), np->key_count, np->key_count, *new_key);
        np->key_count++;
    } else if (left != NULL) {
        // sibling에서 못 가져오면 left sibling에  merge
        merge_nodes(left, np, current_idx - 1);
    } else if (right != NULL) {
        // sibling에서 못 가져오면 right sibling과  merge
        merge_nodes(np, right, current_idx);
    } else {
        // sibling 없을 떄
#ifdef DEBUG
        printf("No siblings and not root. redistribute key\n");
        fflush(stdout);
#endif
    }
}

void redistribute_children(NODE *np) {
    if (np->parent == NULL) {
        // root node 변경
        root = np->children[0];
        root->parent = NULL;
        free_node_mem(np, 0);
        return;
    }
#ifdef DEBUG
    printf("redistribute children\n");
    fflush(stdout);
#endif

    int current_idx = get_current_node_idx(np);
    NODE *left = NULL;
    NODE *right = NULL;
    if (current_idx > 0)
        left = np->parent->children[current_idx - 1];
    if (current_idx < np->parent->children_count - 1)
        right = np->parent->children[current_idx + 1];

    if (left != NULL && left->children_count > minimum_children) {
        // left sibling에게서 가장 마지막 child 1개 가져오기
        NODE **new_child = splice_node(&(left->children), left->children_count, left->children_count - 1, left->children_count, 1);
        left->children_count--;
        // 가져온 child의 parent key 삭제
        splice_key(&(left->keys), left->key_count, left->key_count - 1, left->key_count, 0);
        left->key_count--;
        // child 추가
        insert_node(&(np->children), np->children_count, 0, *new_child);
        np->children_count++;
        free(new_child);
        // parent 키 변경
        int parent_key_idx = current_idx - 1;
        np->parent->keys[parent_key_idx] = create_key(get_smallest_key(np).key);
    } else if (right != NULL && right->children_count > minimum_children) {
        // right sibling에게서 가장 앞 child 1개 가져오기
        NODE **new_child = splice_node(&(right->children), right->children_count, 0, 1, 1);
        right->children_count--;
        // 가져온 child의 parent key 삭제
        splice_key(&(right->keys), right->key_count, 0, 1, 0);
        right->key_count--;
        // child 추가
        insert_node(&(np->children), np->children_count, np->children_count, *new_child);
        np->children_count++;
        free(new_child);
        // parent 키 변경
        int parent_key_idx = current_idx;
        np->parent->keys[parent_key_idx] = create_key(get_smallest_key(right).key);
    } else if (left != NULL) {
        // sibling에서 못 가져오면 left sibling에  merge
        merge_nodes(left, np, current_idx - 1);
    } else if (right != NULL) {
        // sibling에서 못 가져오면 right sibling과  merge
        merge_nodes(np, right, current_idx);
    } else {
#ifdef DEBUG
        printf("No siblings and not root. redistribute children\n");
        fflush(stdout);
#endif
    }
}

void delete_key_tree(NODE *np, KEY key) {
    // Leaf 에서부터 위로 key 지워나가기
    int delete_key_idx = delete_key_current_node(np, key);
    if (np->children == NULL) {
        // Check minimum key count
        printf("key_count %d, minimum_key %d\n", np->key_count, minimum_key);
        fflush(stdout);
        if (np->key_count < minimum_key)
            redistribute_key(np);
    } else {
        // 삭제 key 우측 descendant leaf node의 최소값 추가
        // parent key가 없고 children이 있을 경우 descendent leaft node의 최소값 추가(merge로 인한 결과)
        if (delete_key_idx >= 0 || np->key_count == 0) {
            KEY new_key = get_smallest_key(np->children[delete_key_idx + 1]);
            new_key.data = NULL;
            insert_key_current_node(np, new_key, NULL);
        }

        // Check minimum children count
        if (np->children_count < minimum_children)
            redistribute_children(np);
    }

    if (np->parent != NULL) {
        delete_key_tree(np->parent, key);
    } else {
        // 재귀 종료
        return;
    }
}
