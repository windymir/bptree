/*
    Author: Soeun Kim
    Date: 3, Mar, 2020
    Description: B+ tree main
*/

#include "bptree.h"


extern NODE *root;


int main() {
    set_maximum_key(3);

    insert_data(10, 100);
    printf("%d\n", *get_data(create_key(10)));
    show_tree(root);
    fflush(stdout);
    insert_data(20, 200);
    printf("%d\n", *get_data(create_key(20)));
    show_tree(root);
    fflush(stdout);
    insert_data(30, 300);
    printf("%d\n", *get_data(create_key(30)));
    fflush(stdout);
    show_tree(root);
    insert_data(40, 400);
    printf("%d\n", *get_data(create_key(40)));
    fflush(stdout);
    show_tree(root);
    insert_data(50, 500);
    show_tree(root);
    printf("%d\n", *get_data(create_key(50)));
    fflush(stdout);
    insert_data(23, 230);
    printf("%d\n", *get_data(create_key(23)));
    fflush(stdout);
    show_tree(root);
    insert_data(33, 330);
    printf("%d\n", *get_data(create_key(33)));
    fflush(stdout);
    show_tree(root);
    insert_data(34, 340);
    printf("%d\n", *get_data(create_key(34)));
    fflush(stdout);
    show_tree(root);
    insert_data(36, 360);
    printf("%d\n", *get_data(create_key(36)));
    fflush(stdout);
    show_tree(root);
    insert_data(19, 190);
    printf("%d\n", *get_data(create_key(19)));
    fflush(stdout);
    show_tree(root);
}
