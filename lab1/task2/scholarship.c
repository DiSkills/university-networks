#define _GNU_SOURCE
#include <stdio.h>

#include "scholarship.h"

static int min(const int *arr, int size)
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

static const char *scholarship_msg(const int *marks, int size)
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

static int count_debts(const int *marks, int size)
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

char *scholarship(const char *name, const int *marks, int size)
{
    char *str;

    asprintf(&str, "Student: %s.\nScholarship is %s.\nNumber of debts: %d\n",
             name, scholarship_msg(marks, size), count_debts(marks, size));
    return str;
}
