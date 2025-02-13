#include <fcntl.h>
#include <stdio.h>

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

struct student {
    char *name;
    char **marks;
};

struct client {
    const char *requests, *responses;
};

static int send_request(const struct client *cl, const struct student *st)
{
    int i;
    FILE *f;

    f = fopen(cl->requests, "w");
    if (!f) {
        perror(cl->requests);
        return 0;
    }

    fprintf(f, "%s\n", st->name);
    for (i = 0; i < NUMBER_MARKS; i++) {
        fprintf(f, "%s\n", st->marks[i]);
    }

    fclose(f);
    return 1;
}

static void receive_response(const struct client *cl)
{
    int c;
    FILE *fresp;

    fresp = fopen(cl->responses, "r");
    if (!fresp) {
        perror(cl->responses);
        return;
    }

    while ((c = fgetc(fresp)) != EOF) {
        putchar(c);
    }

    fclose(fresp);
}

int main(int argc, char **argv)
{
    int ok;
    struct client cl;
    struct student st;

    if (argc != 4 + NUMBER_MARKS) {
        fprintf(stderr, "Expected: %s <requests> <responses> "
                "<full-name> <marks>[%d]\n", argv[0], NUMBER_MARKS);
        return 1;
    }

    cl.requests = argv[1];
    cl.responses = argv[2];
    st.name = argv[3];
    st.marks = argv + 4;

    ok = send_request(&cl, &st);
    if (!ok) {
        return 2;
    }
    receive_response(&cl);
    return 0;
}
