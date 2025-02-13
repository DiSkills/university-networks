#include <stdio.h>

#include "scholarship.h"

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

#ifndef NAME_SIZE
#define NAME_SIZE 120
#endif

enum statuses {
    status_ok,
    status_no,
};

struct request {
    char name[NAME_SIZE];
    int marks[NUMBER_MARKS];
};

struct response {
    enum statuses status;

    struct scholarship scholarship;
};

struct server {
    const char *requests, *responses;

    char buffer[sizeof(struct request)];
    int buffer_usage;
};

static void server_handle_request(const struct server *serv)
{
    struct request *req;
    struct response resp;

    if (serv->buffer_usage != sizeof(serv->buffer)) {
        /* TODO: send status */
        return;
    }

    req = (struct request *)serv->buffer;
    resp.status = status_ok;
    resp.scholarship = scholarship(req->name, req->marks, NUMBER_MARKS);
    /* TODO: send response */
}

static int server_run(struct server *serv)
{
    FILE *req;

    req = fopen(serv->requests, "r");
    if (!req) {
        perror(serv->requests);
        return 2;
    }

    for (;;) {
        int c = fgetc(req);
        if (c == EOF) {
            server_handle_request(serv);

            serv->buffer_usage = 0;
            fclose(req);
            req = fopen(serv->requests, "r");
            continue;
        }

        if (serv->buffer_usage < sizeof(serv->buffer)) {
            serv->buffer[serv->buffer_usage] = c;
        }
        serv->buffer_usage++;
    }

    fclose(req);
}

int main(int argc, char **argv)
{
    struct server serv;

    if (argc != 3) {
        fprintf(stderr, "Expected: %s <requests> <responses>\n", argv[0]);
        return 1;
    }

    serv.requests = argv[1];
    serv.responses = argv[2];
    serv.buffer_usage = 0;

    return server_run(&serv);
}
