#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char **argv)
{
    int ok;
    long port;
    char *endptr;
    struct server serv;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        return 1;
    }

    port = strtol(argv[1], &endptr, 10);
    if (!*argv[1] || *endptr) {
        fprintf(stderr, "The port is invalid\n");
        return 2;
    }

    ok = server_init(&serv, port);
    if (!ok) {
        return 3;
    }
    return server_run(&serv);
}
