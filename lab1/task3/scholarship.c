#define _GNU_SOURCE
#include <string.h>

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

static enum scholarship_types scholarship_type(const int *marks, int size)
{
    int mn = min(marks, size);

    switch (mn) {
    case 5:
        return scholarship_increased;
    case 4:
        return scholarship_regular;
    case 3:
        return scholarship_no;
    }
    return scholarship_debts;
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

struct scholarship scholarship(const char *name, const int *marks, int size)
{
    struct scholarship sc;

    strncpy(sc.name, name, sizeof(sc.name));
    sc.type = scholarship_type(marks, size);
    sc.debts = count_debts(marks, size);
    return sc;
}
