/*
    Author: Soeun Kim
    Date: 28, Feb, 2020
    Description: B+ tree KEY header
*/

typedef int DATA;

typedef struct key {
    int key;
    // For leaf node only
    DATA *data;
} KEY;

int compare_key(KEY a, KEY b);
int is_key_equals(KEY a, KEY b);
void insert_key(KEY **array, int length, int insert_idx, KEY* value);
void free_key_mem(KEY **key);
KEY *splice_key(KEY **array, int length, int splice_start_idx, int splice_end_idx, int get_spliced_key);
