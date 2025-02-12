#include <fcntl.h>
#include <stdio.h>

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

static int send_request(const char *requests, const char *name, char **marks)
{
    int i;
    FILE *f;

    f = fopen(requests, "w");
    if (!f) {
        perror(requests);
        return 0;
    }

    fprintf(f, "%s\n", name);
    for (i = 0; i < NUMBER_MARKS; i++) {
        fprintf(f, "%s\n", marks[i]);
    }
    fflush(f);

    fclose(f);
    return 1;
}

int main(int argc, char **argv)
{
    int ok;
    char *requests, *responses, *name, **marks;

    if (argc != 4 + NUMBER_MARKS) {
        fprintf(stderr, "Expected: %s <requests> <responses> "
                "<full-name> <marks>[%d]\n", argv[0], NUMBER_MARKS);
        return 1;
    }

    requests = argv[1];
    responses = argv[2];
    name = argv[3];
    marks = argv + 4;

    ok = send_request(requests, name, marks);
    if (!ok) {
        return 2;
    }

    return 0;
}
