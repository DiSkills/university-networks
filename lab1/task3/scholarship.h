#ifndef SCHOLARSHIP_H
#define SCHOLARSHIP_H

#ifndef NAME_SIZE
#define NAME_SIZE 120
#endif

enum scholarship_types {
    scholarship_debts,
    scholarship_no,
    scholarship_regular,
    scholarship_increased,
};

struct scholarship {
    char name[NAME_SIZE];
    enum scholarship_types type;
    int debts;
};

struct scholarship scholarship(const char *name, const int *marks, int size);

#endif
