/*
    Author: Soeun Kim
    Date: 28, Feb, 2020
    Description: B+ tree KEY manager
*/

#include "key.h"


bool compare_key(KEY a, KEY b){
    // KEY 타입 변경시 같이 수정 필요함
    return a.key < b.key;
}

bool is_key_equals(KEY a, KEY b) {
    // KEY 타입 변경시 같이 수정 필요함
    return a.key == b.key;
}

bool insert_key(KEY** array, int length, int insert_idx, KEY value) {
    KEY *keys = *array;
    if (length >= maximum_key + 1) {
        // array 최대 키 개수는 maximum_key + 1
        // 현재 array가 꽉 차서 키를 추가할 수 없음
        return false;
    }

    // 지정한 index 이후의 값들을 한칸씩 뒤로 이동
    for (int i = length; i > insert_idx; i--) {
        keys[i] = keys[i - 1];
    }
    keys[insert_idx] = value;
    return true;
}

KEY *splice_key(KEY** array, int length, int splice_start_idx, int splice_end_idx, bool get_spliced_key) {
    KEY *keys = *array;

    int splice_length = splice_end_idx - splice_start_idx;
    if (splice_end_idx > length || splice_length <= 0) {
        // Cannot splice keys
        return NULL;
    }

    KEY *spliced_keys = NULL;
    if (get_spliced_key) {
        spliced_keys = (KEY *)malloc(sizeof(KEY) * splice_length);
        for (int i = splice_start_idx; i < splice_end_idx; i++) {
            spliced_keys[i - splice_start_idx] = keys[i];
        }
    } else {
        for (int i = splice_start_idx; i < splice_end_idx; i++) {
            free(keys[i].data);
        }
    }

    for (int i = splice_start_idx; i < length - splice_length; i++) {
        keys[i] = keys[i + splice_length];
    }

    return spliced_keys;
}

KEY create_key_data(int key, int data) {
    KEY key_data;
    key_data.key = key;
    key_data.data = (DATA *)malloc(sizeof(DATA));
    *(key_data.data) = data;

    return key_data;
}

KEY create_key(int key) {
    KEY key_type;
    key_type.key = key;
    key_type.data = NULL;
    return key_type;
}
