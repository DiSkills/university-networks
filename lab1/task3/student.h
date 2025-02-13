#ifndef STUDENT_H
#define STUDENT_H

#ifndef NAME_SIZE
#define NAME_SIZE 120
#endif

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

struct request_student {
    char name[NAME_SIZE];
    int marks[NUMBER_MARKS];
};

enum scholarship_types {
    scholarship_debts,
    scholarship_no,
    scholarship_regular,
    scholarship_increased,
};

struct response_student {
    char name[NAME_SIZE];
    enum scholarship_types scholarship;
    int debts;
};

struct response_student student(const struct request_student *student);

#endif
