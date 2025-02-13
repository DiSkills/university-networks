#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "student.h"

struct server {
    const char *requests, *responses;
};

enum fsm_states {
    fsm_start,
    fsm_name = fsm_start,
    fsm_mark,
    fsm_finish,
    fsm_error,
};

struct fsm {
    enum fsm_states state;

    struct student student;
};

static void fsm_init(struct fsm *fsm)
{
    fsm->state = fsm_start;
    fsm->student.name = NULL;
    fsm->student.marks_usage = 0;
}

static void fsm_clear(struct fsm *fsm)
{
    fsm->state = fsm_start;
    if (fsm->student.name) {
        free(fsm->student.name);
    }
    fsm->student.name = NULL;
    fsm->student.marks_usage = 0;
}

static void fsm_handle_mark(struct fsm *fsm, char *line)
{
    long mark;
    char *endptr;

    mark = strtol(line, &endptr, 10);
    if (!*line || *endptr || mark < 2 || mark > 5) {
        fsm->state = fsm_error;
        return;
    }

    fsm->student.marks[fsm->student.marks_usage] = mark;
    fsm->student.marks_usage++;
    if (fsm->student.marks_usage >= NUMBER_MARKS) {
        fsm->state = fsm_finish;
        return;
    }
}

static void fsm_step(struct fsm *fsm, char *line)
{
    switch (fsm->state) {
    case fsm_name:
        fsm->student.name = line;
        fsm->state = fsm_mark;
        return;
    case fsm_mark:
        fsm_handle_mark(fsm, line);
        break;
    case fsm_finish:
        break;
    case fsm_error:
        break;
    }
    free(line);
}

static void server_send_response(const struct server *serv, const char *msg)
{
    FILE *fresp = fopen(serv->responses, "w");
    if (!fresp) {
        perror(serv->responses);
        return;
    }
    fputs(msg, fresp);
    fclose(fresp);
}

static void server_send_error(const struct server *serv)
{
    server_send_response(serv, "An error has occurred\n");
}

static void server_handle_request(const struct server *serv, char *request)
{
    char *nl;
    struct fsm fsm;

    fsm_init(&fsm);
    while ((nl = strchr(request, '\n')) != NULL) {
        char *line;

        *nl = '\0';
        line = strdup(request);
        request = nl + 1;

        fsm_step(&fsm, line);
    }

    if (fsm.state == fsm_finish) {
        char *rep = report(&fsm.student);
        server_send_response(serv, rep);
        free(rep);
    } else if (fsm.state == fsm_error) {
        server_send_error(serv);
    }

    fsm_clear(&fsm);
}

static int server_run(const struct server *serv)
{
    FILE *freq;
    struct string *str = str_init();

    freq = fopen(serv->requests, "r");
    if (!freq) {
        perror(serv->requests);
        return 2;
    }

    for (;;) {
        int c = fgetc(freq);
        if (c == EOF) {
            server_handle_request(serv, str->data);

            str_clear(str);
            fclose(freq);
            freq = fopen(serv->requests, "r");
        } else {
            str_append(str, c);
        }
    }

    fclose(freq);

    str_del(str);
    free(str);
    return 0;
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

    return server_run(&serv);
}
