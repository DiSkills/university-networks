#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

enum fsm_states {
    fsm_start,
    fsm_name = fsm_start,
    fsm_mark,
    fsm_finish,
    fsm_error,
};

struct request_fsm {
    enum fsm_states state;

    char *name;

    int marks[NUMBER_MARKS];
    int marks_usage;
};

static void request_fsm_init(struct request_fsm *fsm)
{
    fsm->state = fsm_start;
    fsm->name = NULL;
    fsm->marks_usage = 0;
}

static void request_fsm_clear(struct request_fsm *fsm)
{
    fsm->state = fsm_start;
    if (fsm->name) {
        free(fsm->name);
    }
    fsm->name = NULL;
    fsm->marks_usage = 0;
}

static void request_fsm_handle_mark(struct request_fsm *fsm, char *line)
{
    long mark;
    char *endptr;

    mark = strtol(line, &endptr, 10);
    if (!*line || *endptr || mark < 2 || mark > 5) {
        fsm->state = fsm_error;
        return;
    }

    fsm->marks[fsm->marks_usage] = mark;
    fsm->marks_usage++;
    if (fsm->marks_usage >= NUMBER_MARKS) {
        fsm->state = fsm_finish;
        return;
    }
}

static void request_fsm_step(struct request_fsm *fsm, char *line)
{
    switch (fsm->state) {
    case fsm_name:
        fsm->name = line;
        fsm->state = fsm_mark;
        return;
    case fsm_mark:
        request_fsm_handle_mark(fsm, line);
        break;
    case fsm_finish:
        break;
    case fsm_error:
        break;
    }
    free(line);
}

static void server_handle_request(char *request)
{
    char *nl;
    struct request_fsm fsm;
    request_fsm_init(&fsm);

    while ((nl = strchr(request, '\n')) != NULL) {
        char *line;

        *nl = '\0';
        line = strdup(request);
        request = nl + 1;

        request_fsm_step(&fsm, line);
    }
    if (fsm.state == fsm_finish) {
        /* TODO */
    }

    request_fsm_clear(&fsm);
}

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
            server_handle_request(str->data);

            str_clear(str);
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
