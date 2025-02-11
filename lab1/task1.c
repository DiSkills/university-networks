#include <stdio.h>
#include <stdlib.h>

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

int strs_to_ints(char **strings, int *arr, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        char *endptr;
        long number;

        number = strtol(strings[i], &endptr, 10);
        if (!*strings[i] || *endptr) {
            return 0;
        }

        arr[i] = number;
    }
    return 1;
}

int min(const int *arr, int size)
{
    int i, mn;

    mn = arr[0];
    for (i = 1; i < size; i++) {
        if (arr[i] < mn) {
            mn = arr[i];
        }
    }
    return mn;
}

const char *scholarship_msg(const int *marks, int size)
{
    int mn = min(marks, size);

    switch (mn) {
    case 5:
        return "increased";
    case 4:
        return "regular";
    case 3:
        return "not";
    }
    return "not, and you have debts";
}

int count_debts(const int *marks, int size)
{
    int i, count;

    count = 0;
    for (i = 0; i < size; i++) {
        if (marks[i] < 3) {
            count++;
        }
    }
    return count;
}

int main(int argc, char **argv)
{
    int marks[NUMBER_MARKS];
    const char *scholarship;

    if (argc != 2 + NUMBER_MARKS) {
        fprintf(stderr, "Expected: %s <full-name> <marks>[%d]\n",
                argv[0], NUMBER_MARKS);
        return 1;
    }

    int ok = strs_to_ints(argv + 2, marks, NUMBER_MARKS);
    if (!ok) {
        fprintf(stderr, "The marks are incorrect\n");
        return 2;
    }
    scholarship = scholarship_msg(marks, NUMBER_MARKS);

    printf("Student: %s.\nScholarship is %s.\n", argv[1], scholarship);
    printf("Number of debts: %d\n", count_debts(marks, NUMBER_MARKS));
    return 0;
}
