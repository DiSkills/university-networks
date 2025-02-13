#include <string.h>

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

static enum scholarship_types scholarship(const struct request_student *st)
{
    int mn = min(st->marks, NUMBER_MARKS);

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

static int count_debts(const struct request_student *st)
{
    int i, count;

    count = 0;
    for (i = 0; i < NUMBER_MARKS; i++) {
        if (st->marks[i] < 3) {
            count++;
        }
    }
    return count;
}

struct response_student student(const struct request_student *student)
{
    struct response_student st;

    strncpy(st.name, student->name, sizeof(st.name));
    st.scholarship = scholarship(student);
    st.debts = count_debts(student);
    return st;
}
