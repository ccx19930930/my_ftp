#ifndef __EPOLL_H__
#define __EPOLL_H__

#include"head.h"

int epoll_init(int sfd);
void epoll_add(int epfd,int fd);
void epoll_delete(int epfd,int fd);


#endif
