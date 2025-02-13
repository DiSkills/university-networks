#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

#ifndef NAME_SIZE
#define NAME_SIZE 120
#endif

struct request {
    char name[NAME_SIZE];
    int marks[NUMBER_MARKS];
};

static int strs_to_ints(char **strings, int *arr, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        long number;
        char *endptr;

        number = strtol(strings[i], &endptr, 10);
        if (!*strings[i] || *endptr) {
            return 0;
        }

        arr[i] = number;
    }
    return 1;
}

static int request_init(struct request *req, char *name, char **str_marks)
{
    strncpy(req->name, name, sizeof(req->name));
    return strs_to_ints(str_marks, req->marks, NUMBER_MARKS);
}

int main(int argc, char **argv)
{
    int ok;
    struct request req;
    char *name, **marks;

    if (argc != 4 + NUMBER_MARKS) {
        fprintf(stderr, "Expected: %s <requests> <responses> "
                "<full-name> <marks>[%d]\n", argv[0], NUMBER_MARKS);
        return 1;
    }

    name = argv[3];
    marks = argv + 4;

    ok = request_init(&req, name, marks);
    if (!ok) {
        fprintf(stderr, "The marks are incorrect\n");
        return 2;
    }
    return 0;
}
