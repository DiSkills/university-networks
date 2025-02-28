#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "session.h"
#include "student.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 2048
#endif

enum fsm_states {
    fsm_start,
    fsm_name = fsm_start,
    fsm_mark,
    fsm_finish,
    fsm_error,
};

struct session {
    int fd;

    char buffer[BUFFER_SIZE];
    int buffer_usage;

    enum fsm_states state;
    struct student student;
};

static void session_send_string(const struct session *sess, const char *str)
{
    write(sess->fd, str, strlen(str));
}

struct session *session_init(int fd)
{
    struct session *sess = malloc(sizeof(*sess));

    sess->fd = fd;
    sess->buffer_usage = 0;
    sess->state = fsm_start;
    sess->student.name = NULL;
    sess->student.marks_usage = 0;

    session_send_string(sess, "Input the student's name:\n");
    return sess;
}

void session_del(struct session *sess)
{
    sess->fd = -1;
    sess->buffer_usage = 0;
    sess->state = fsm_start;
    if (sess->student.name) {
        free(sess->student.name);
        sess->student.name = NULL;
    }
    sess->student.marks_usage = 0;
}

static void session_handle_mark(struct session *sess, char *line)
{
    long mark;
    char *endptr;

    mark = strtol(line, &endptr, 10);
    if (!*line || *endptr || mark < 2 || mark > 5) {
        session_send_string(sess, "The mark is invalid. Try again:\n");
        return;
    }

    sess->student.marks[sess->student.marks_usage] = mark;
    sess->student.marks_usage++;
    if (sess->student.marks_usage >= NUMBER_MARKS) {
        char *msg = report(&sess->student);
        session_send_string(sess, msg);
        free(msg);

        free(sess->student.name);
        sess->student.name = NULL;
        sess->student.marks_usage = 0;
        sess->state = fsm_start;
        session_send_string(sess, "Input the student's name:\n");
    }
}

static void session_fsm_step(struct session *sess, char *line)
{
    switch (sess->state) {
    case fsm_name:
        sess->student.name = line;
        sess->state = fsm_mark;
        session_send_string(sess, "Input four marks:\n");
        return;
    case fsm_mark:
        session_handle_mark(sess, line);
        break;
    case fsm_finish:
        break;
    case fsm_error:
        break;
    }
    free(line);
}

static void session_check_lf(struct session *sess)
{
    char *nl;

    while ((nl = memchr(sess->buffer, '\n', sess->buffer_usage)) != NULL) {
        char *line;
        int pos;

        pos = nl - sess->buffer;
        if (pos > 0 && sess->buffer[pos - 1] == '\r') {
            sess->buffer[pos - 1] = '\0';
        }
        sess->buffer[pos] = '\0';

        line = strdup(sess->buffer);
        sess->buffer_usage -= pos + 1;
        memmove(sess->buffer, nl + 1, sess->buffer_usage);
        session_fsm_step(sess, line);
    }
}

int session_receive(struct session *sess)
{
    int rc = read(sess->fd, sess->buffer + sess->buffer_usage,
            sizeof(sess->buffer) - sess->buffer_usage);
    if (rc == -1) {
        sess->state = fsm_error;
        return 0;
    }
    if (rc == 0) {
        sess->state = fsm_finish;
        return 0;
    }
    sess->buffer_usage += rc;

    session_check_lf(sess);
    if (sess->buffer_usage >= sizeof(sess->buffer)) {
        session_send_string(sess, "Line is too long...\n");
        sess->state = fsm_error;
        return 0;
    }
    return 1;
}
