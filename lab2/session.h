#ifndef SESSION_H
#define SESSION_H

struct session *session_init(int fd);
void session_del(struct session *sess);
int session_receive(struct session *sess);

#endif
