#include <stdlib.h>

#include "str.h"

struct string *str_init()
{
    struct string *str = malloc(sizeof(*str));

    str->data = malloc(STR_MIN_LENGTH);
    *str->data = '\0';

    str->length = 0;
    str->max_size = STR_MIN_LENGTH;

    return str;
}

void str_clear(struct string *str)
{
    *str->data = '\0';
    str->length = 0;
}

void str_del(struct string *str)
{
    free(str->data);
    str->data = NULL;
    str->length = 0;
    str->max_size = 0;
}

static int str_is_full(const struct string *str)
{
    return str->length + 1 == str->max_size;
}

void str_append(struct string *str, char c)
{
    if (str_is_full(str)) {
        str->max_size *= 2;
        str->data = realloc(str->data, str->max_size);
    }

    str->data[str->length] = c;
    str->length++;
    str->data[str->length] = '\0';
}
