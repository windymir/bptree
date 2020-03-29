/*
    Author: Soeun Kim
    Date: 3, Mar, 2020
    Description: B+ tree main
*/
#include "bptree_ui.h"


int main() {
    printf("B+ Tree를 시작합니다.\n");

    initialize();

    while(1) {
        print_option();

        void *check_eof = fgets(input_buffer, INPUT_BUFFER, stdin);
        if (check_eof == NULL) {
            return 0;
        }

        int command = atoi(input_buffer);
        switch(command) {
            case 1:
                show_tree(root);
                break;
            case 2:
                input();
                break;
            case 3:
                delete();
                break;
            case 4:
                data();
                break;
            case 5:
                search();
                break;
            case 6:
                clear();
                break;
            case 7:
                example_tree();
                break;
            default:
                if (is_exit(input_buffer))
                    return 0;
                fflush(stdin);
                printf("올바른 선택이 아닙니다.");
        }
    }

    return -1;
}
