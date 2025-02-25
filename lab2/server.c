#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "server.h"

int server_init(struct server *serv, int port)
{
    int i, lsd, res, opt;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, 0);
    if (lsd == -1) {
        perror("socket");
        return 0;
    }

    opt = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    res = bind(lsd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        return 0;
    }

    listen(lsd, LISTEN_QLEN);
    serv->lsd = lsd;

    serv->session_array =
        malloc(INIT_SESSION_ARRAY_SIZE * sizeof(*serv->session_array));
    serv->session_array_size = INIT_SESSION_ARRAY_SIZE;
    for (i = 0; i < serv->session_array_size; i++) {
        serv->session_array[i] = NULL;
    }
    return 1;
}
