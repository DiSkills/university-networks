#ifndef STUDENT_H
#define STUDENT_H

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

struct student {
    char *name;
    int marks[NUMBER_MARKS];
    int marks_usage;
};

char *report(const struct student *st);

#endif
