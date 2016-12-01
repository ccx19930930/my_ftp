#include"func.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
p_wait_queue phead;
p_wait_queue ptail;

int main(int argc,char *argv[])
{
	if(3!=argc)
	{
		printf("error argc!\n");
		return -1;
	}
	int num;
	int port;
	num=atoi(argv[2]);
	port=atoi(argv[1]);
	queue_init(num);
	pthread_t *pthid;
	pthread_init(pthid,num);
	int sfd;
	sfd=tcp_init(port,M);
	int epfd;
	epfd=epoll_init(sfd);
	struct epoll_event *event;
	event=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(M+1));
	int ret;
	int i;
	int new_fd;
	int temp;
	while(1)
	{
		memset(event,0,sizeof(struct epoll_event)*(M+1));
		ret=epoll_wait(epfd,event,M+1,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(event[i].events==EPOLLIN)
				{
					if(event[i].data.fd==sfd)
					{
						new_fd=accept(sfd,NULL,NULL);
						if(-1==new_fd)
						{
							perror("accept");
							return -1;
						}
						epoll_add(epfd,new_fd);
					}else{

						temp=verify_password(event[i].data.fd);
						switch(temp)
						{
							case -1:
							case 0:
								{
									epoll_delete(epfd,event[i].data.fd);
									close(event[i].data.fd);
									break;
								}
							case 1:
								{
									printf("password success!\n");
									enqueue(event[i].data.fd);
									pthread_cond_signal(&cond);
									break;
								}



						}
					}
				}
			}
		}
	}
}
