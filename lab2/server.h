#ifndef SERVER_H
#define SERVER_H

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 16
#endif

#ifndef INIT_SESSION_ARRAY_SIZE
#define INIT_SESSION_ARRAY_SIZE 16
#endif

struct server {
    int lsd;

    void **session_array;
    int session_array_size;
};

int server_init(struct server *serv, int port);

#endif
