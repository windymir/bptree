/*
    Author: Soeun Kim
    Date: 28, Feb, 2020
    Description: B+ tree KEY manager
*/
#include "key.h"


bool compare_key(KEY a, Key b){
    // KEY 타입 변경시 같이 수정 필요함
    return a.key < b.key;
}

bool is_key_equals(KEY a, KEY b) {
    // KEY 타입 변경시 같이 수정 필요함
    return a.key == b.key;
}

void insert_key(KEY** array, int length, int insert_idx, KEY* value) {
    KEY *keys = *array;
    for (int i = length; i > insert_idx; i--) {
        keys[i] = keys[i - 1];
    }
    keys[insert_idx] = *value;
}

void free_key_mem(KEY **key) {
    if ((**key).data != NULL)
        free((**key).data);
    free(*key);
    *key = NULL;
}

KEY *splice_key(KEY** array, int length, int splice_start_idx, int splice_end_idx, bool get_spliced_key) {
    KEY *keys = *array;

    int splice_length = splice_end_idx - splice_start_idx;
    if (splice_end_idx > length || splice_length <= 0)
        // Cannot splice keys
        return NULL;

    KEY *spliced_keys = NULL;
    if (get_spliced_key) {
        spliced_keys = (KEY *)malloc(sizeof(KEY) * splice_length);
        for (int i = splice_start_idx; i < splice_end_idx; i++) {
            spliced_keys[i - splice_start_idx] = keys[i];
        }
    }

    for (int i = splice_start_idx; i < length - splice_length; i++) {
        keys[i] = keys[i + splice_length];
    }

    return spliced_keys;
}
