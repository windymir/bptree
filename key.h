/*
    Author: Soeun Kim
    Date: 28, Feb, 2020
    Description: B+ tree KEY header
*/
#ifndef KEY_H
#define KEY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef int DATA;

typedef struct key {
    int key;
    // For leaf node only
    DATA *data;
} KEY;

extern int maximum_key;
extern int minimum_key;


bool compare_key(KEY a, KEY b);
bool is_key_equals(KEY a, KEY b);
bool insert_key(KEY **array, int length, int insert_idx, KEY value);
KEY *splice_key(KEY **array, int length, int splice_start_idx, int splice_end_idx, bool get_spliced_key);
KEY create_key_data(int key, int data);
KEY create_key(int key);

#endif // KEY_H
