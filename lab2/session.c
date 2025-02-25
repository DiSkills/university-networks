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

struct session *session_init(int fd)
{
    struct session *sess = malloc(sizeof(*sess));

    sess->fd = fd;
    sess->buffer_usage = 0;
    sess->state = fsm_start;
    sess->student.name = NULL;
    sess->student.marks_usage = 0;

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

static void session_send_string(const struct session *sess, const char *str)
{
    write(sess->fd, str, strlen(str));
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
    /* TODO: check */

    if (sess->buffer_usage >= sizeof(sess->buffer)) {
        session_send_string(sess, "Line is too long...\n");
        sess->state = fsm_error;
        return 0;
    }
    return 1;
}
