/*
    Author: Soeun Kim
    Date: 29, Mar, 2020
    Description: B+ tree UI Header
*/
#ifndef BPTREE_UI_H
#define BPTREE_UI_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

#include "bptree.h"

#define INPUT_BUFFER 5
#define MINIMUM_MAX_KEY 2
#define MAXIMUM_MAX_KEY 99

extern char input_buffer[INPUT_BUFFER];


void print_option();
void initialize();
void clear();
void *input_key(char *inform, int *result);
void input();
void delete();
void data();
void search();
bool is_exit(char *input_buffer);
void example_tree();


#endif // BPTREE_UI_H
