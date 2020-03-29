/*
    Author: Soeun Kim
    Date: 29, Mar, 2020
    Description: B+ tree UI
*/
#include "bptree_ui.h"


char input_buffer[INPUT_BUFFER];

void print_option() {
    char *option[] = {
        "\n==============================\n",
        "1 : Tree 현황 보기\n",
        "2 : Tree에 추가하기\n",
        "3 : Tree에서 삭제하기\n",
        "4 : Data 가져 오기\n",
        "5 : Tree 검색\n",
        "6 : Tree 초기화\n",
        "7 : Sample Tree\n",
        "exit : 프로그램 종료\n",
        "==============================\n",
        "> ",
    };

    for (int i = 0; i < 11; i++)
        printf("%s", option[i]);
}

void initialize() {
    int max_key_count;
    while(1) {
        printf("Node당 최대 키 개수를 입력해주세요(%d ~ %d): ", MINIMUM_MAX_KEY, MAXIMUM_MAX_KEY);
        char *check_eof = fgets(input_buffer, INPUT_BUFFER, stdin);
        if (check_eof == NULL) {
            return;
        }

        max_key_count = atoi(input_buffer);
        if (max_key_count < MINIMUM_MAX_KEY || max_key_count > MAXIMUM_MAX_KEY) {
            fflush(stdin);
            printf("키 개수는 2 ~ 99 사이의 정수값을 입력해주세요.\n");
            continue;
        }

        initialize_tree(max_key_count);
        return;
    }
}

void clear() {
    printf("현재 Tree를 모두 지우시겠습니까? (y/n)\n");
    char *input = fgets(input_buffer, INPUT_BUFFER, stdin);
    if (input == NULL || strcasecmp(input_buffer, "y") != 0) {
        // Cancel clear
        return;
    }

    clear_tree();
    initialize();
}

void *input_key(char *inform, int *result) {
    while (1) {
        printf("%s", inform);
        char *input = fgets(input_buffer, INPUT_BUFFER, stdin);
        if (input == NULL) {
            // EOF
            return NULL;
        }

        *result = atoi(input_buffer);
        if (*result == 0 && strcmp(input_buffer, "0") != 0) {
            fflush(stdin);
            printf("잘못된 Key 형식입니다. int값을 입력하세요.\n");
        } else {
            break;
        }
    }

    return result;
}

void input() {
    int key, data;

    void *check_eof = input_key("\nTree에 추가할 Key를 입력하세요(int): ", &key);
    if (check_eof == NULL) {
        return;
    }

    while (1) {
        printf("저장할 Data를 입력하세요(int): ");

        check_eof = fgets(input_buffer, INPUT_BUFFER, stdin);
        if (check_eof == NULL) {
            return;
        }

        data = atoi(input_buffer);
        if (data == 0 && strcmp(input_buffer, "0") != 0) {
            fflush(stdin);
            printf("잘못된 Data 형식입니다. int값을 입력하세요.\n");
        }  else {
            break;
        }
    }

    insert_data(key, data);
}

void delete() {
    int key;
    void *check_eof = input_key("\n삭제할 Key를 입력하세요(int): ", &key);
    if (check_eof == NULL) {
        return;
    }

    delete_data(key);
}

void data() {
    int key;
    void *check_eof = input_key("\nData를 가져올 Key를 입력하세요(int): ", &key);
    if (check_eof == NULL) {
        return;
    }

    DATA *data = get_data(create_key(key));
    if (data == NULL) {
        printf("해당하는 key가 없습니다.\n");
    }
    else
        printf("Key %d 의 data는 %d 입니다.\n", key, *data);
}

void search() {
    int from, to;
    void *check_eof = input_key("\n범위 검색 시작 Key를 입력하세요(int): ", &from);
    if (check_eof == NULL) {
        return;
    }

    check_eof = input_key("\n범위 검색 끝 Key를 입력하세요(int): ", &to);
    if (check_eof == NULL) {
        return;
    }

    range_search(from, to);
}

bool is_exit(char *input_buffer) {
    if (strcasecmp(input_buffer, "exit") == 0) {
        return 1;
    }
    else
        return 0;
}

void example_tree() {
    insert_data(10, 100);
    insert_data(20, 200);
    insert_data(30, 300);
    insert_data(40, 400);
    insert_data(50, 500);
    insert_data(23, 230);
    insert_data(33, 330);
    insert_data(34, 340);
    insert_data(36, 360);
    insert_data(19, 190);
}
