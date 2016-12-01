#include"func.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
p_wait_queue phead;
p_wait_queue ptail;
int epfd;

int main()
{
	conf_struct conf;
	get_conf(&conf);
	queue_init();
	pthread_t *pthid;
	pthread_init(pthid,conf.pthread_num);
	int sfd;
	sfd=tcp_init(conf.port,M);
	epfd=epoll_init(sfd);
	struct epoll_event *event;
	event=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(M+1));
	int ret;
	int i;
	int new_fd;
	int temp;
	p_wait_queue p;
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
						p=(p_wait_queue)calloc(1,sizeof(wait_queue));
						p->sfd=event[i].data.fd;
						temp=verify_password(p);
						switch(temp)
						{
							case -1:
							case 0:
								{
									epoll_delete(epfd,event[i].data.fd);
									close(event[i].data.fd);
									free(p);
									printf("(main)close = %d\n",event[i].data.fd);
									break;
								}
							case 1:
								{
									printf("password success!\n");

									pthread_mutex_lock(&mutex);
									enqueue(p);
									pthread_mutex_unlock(&mutex);
									pthread_cond_signal(&cond);
									epoll_delete(epfd,event[i].data.fd);
									break;
								}

						}
					}
				}
			}
		}
	}
}
