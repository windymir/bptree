/*
    Author: Soeun Kim
    Date: 27, Feb, 2020
    Description: B+ tree header
*/
#ifndef BPTREE_H
#define BPTREE_H

#include <stdio.h>
#include <stdlib.h>

#include "key.h"
#include "node.h"


void set_maximum_key(int order);
DATA *get_data(KEY key);
void insert_data(int key, DATA data);
void delete_data(KEY key);
void range_search(int min, int max);
void show_tree();

#endif
