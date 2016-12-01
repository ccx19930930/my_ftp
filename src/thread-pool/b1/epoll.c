#include"func.h"


void epoll_init()//epoll相关初始化
{
	epfd=epoll_create(1);
	epoll_add(epfd,sfd);
}
void epoll_add(int epfd,int fd)
{
	int ret;
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl_add");
	}else{
		printf("%d epoll_add %d success!\n",epfd,fd);//测试
	}
}
void epoll_delete(int epfd,int fd)
{
	int ret;
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl_delete");
	}else{
		close(fd);
		printf("%d epoll_delete %d success!\n",epfd,fd);//测试
	}
}
