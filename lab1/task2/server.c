#include <stdio.h>
#include <stdlib.h>

#include "str.h"

static int server_run(char *requests, char *responses)
{
    FILE *req;
    struct string *str = str_init();

    req = fopen(requests, "r");
    if (!req) {
        perror(requests);
        return 2;
    }

    for (;;) {
        int c = fgetc(req);
        if (c == EOF) {
            /* TODO: handle the request */

            fclose(req);
            req = fopen(requests, "r");
        } else {
            str_append(str, c);
        }
    }

    fclose(req);

    str_del(str);
    free(str);
}

int main(int argc, char **argv)
{
    char *requests, *responses;
    if (argc != 3) {
        fprintf(stderr, "Expected: %s <requests> <responses>\n", argv[0]);
        return 1;
    }

    requests = argv[1];
    responses = argv[2];

    return server_run(requests, responses);
}
