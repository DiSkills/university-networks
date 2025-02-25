#define _GNU_SOURCE
#include <stdio.h>

#include "student.h"

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

static const char *scholarship(const struct student *st)
{
    int mn = min(st->marks, st->marks_usage);

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

static int count_debts(const struct student *st)
{
    int i, count;

    count = 0;
    for (i = 0; i < st->marks_usage; i++) {
        if (st->marks[i] < 3) {
            count++;
        }
    }
    return count;
}

char *report(const struct student *st)
{
    char *str;

    asprintf(&str, "Student: %s.\nScholarship is %s.\nNumber of debts: %d\n",
             st->name, scholarship(st), count_debts(st));
    return str;
}
