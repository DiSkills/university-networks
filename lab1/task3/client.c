#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    struct {
        char name[NAME_SIZE];
        int marks[NUMBER_MARKS];
    } student;
};

enum scholarship_types {
    scholarship_debts,
    scholarship_no,
    scholarship_regular,
    scholarship_increased,
};

struct response {
    enum statuses status;

    struct {
        char name[NAME_SIZE];
        enum scholarship_types scholarship;
        int debts;
    } student;
};

struct client {
    const char *requests, *responses;

    char buffer[sizeof(struct request)];
    int buffer_usage;
};

static const char *scholarship_msg(enum scholarship_types scholarship)
{
    switch (scholarship) {
    case scholarship_increased:
        return "increased";
    case scholarship_regular:
        return "regular";
    case scholarship_no:
        return "not";
    case scholarship_debts:
        return "not, and you have debts";
    }
    return NULL;
}

static int strs_to_ints(char **strings, int *arr, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        long number;
        char *endptr;

        number = strtol(strings[i], &endptr, 10);
        if (!*strings[i] || *endptr) {
            return 0;
        }

        arr[i] = number;
    }
    return 1;
}

static int request_init(struct request *req, const char *name,
                        char **str_marks)
{
    strncpy(req->student.name, name, sizeof(req->student.name));
    return strs_to_ints(str_marks, req->student.marks, NUMBER_MARKS);
}

static void receive_response(struct client *cl)
{
    int c;
    FILE *fresp;
    struct response *resp;

    fresp = fopen(cl->responses, "r");
    if (!fresp) {
        perror(cl->responses);
        return;
    }

    while ((c = fgetc(fresp)) != EOF) {
        if (cl->buffer_usage < sizeof(cl->buffer)) {
            cl->buffer[cl->buffer_usage] = c;
        }
        cl->buffer_usage++;
    }

    resp = (struct response *)cl->buffer;
    if (resp->status == status_ok) {
        const char *scholarship = scholarship_msg(resp->student.scholarship);
        printf("Student: %s.\nScholarship is %s.\nNumber of debts: %d\n",
               resp->student.name, scholarship, resp->student.debts);
    } else {
        printf("An error has occurred\n");
    }

    fclose(fresp);
}

static int send_request(struct client *cl, const struct request *req)
{
    int fd = open(cl->requests, O_WRONLY);
    if (fd == -1) {
        perror(cl->requests);
        return 0;
    }
    write(fd, req, sizeof(*req));
    close(fd);
    return 1;
}

int main(int argc, char **argv)
{
    int ok;
    struct client cl;
    struct request req;
    char *name, **marks;

    if (argc != 4 + NUMBER_MARKS) {
        fprintf(stderr, "Expected: %s <requests> <responses> "
                "<full-name> <marks>[%d]\n", argv[0], NUMBER_MARKS);
        return 1;
    }

    cl.requests = argv[1];
    cl.responses = argv[2];
    cl.buffer_usage = 0;

    name = argv[3];
    marks = argv + 4;

    ok = request_init(&req, name, marks);
    if (!ok) {
        fprintf(stderr, "The marks are incorrect\n");
        return 2;
    }
    ok = send_request(&cl, &req);
    if (!ok) {
        return 3;
    }
    receive_response(&cl);
    return 0;
}
