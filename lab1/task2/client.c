#include <stdio.h>

#if !defined(NUMBER_MARKS) || NUMBER_MARKS < 1
#define NUMBER_MARKS 4
#endif

int main(int argc, char **argv)
{
    if (argc != 4 + NUMBER_MARKS) {
        fprintf(stderr, "Expected: %s <requests> <responses> "
                "<full-name> <marks>[%d]\n", argv[0], NUMBER_MARKS);
        return 1;
    }
    return 0;
}

