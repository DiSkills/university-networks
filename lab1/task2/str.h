#ifndef STR_H
#define STR_H

#define STR_MIN_LENGTH 16

struct string {
    char *data;
    int length;
    int max_size;
};

struct string *str_init();
void str_clear(struct string *str);
void str_del(struct string *str);
void str_append(struct string *str, char c);

#endif
