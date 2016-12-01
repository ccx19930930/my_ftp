#include"func.h"
p_wait_queue wait_phead;
p_wait_queue wait_ptail; 
p_ready_queue ready_phead;
p_ready_queue ready_ptail;
pthread_mutex_t mutex;
pthread_mutex_t mutex_ready;
pthread_cond_t ready_cond;
int sfd;
int new_fd;
int t_fd;
int epfd;
pthread_t pthid[N];
pthread_t pthid_ready;
pthread_t pthid_wait;








int main()
{
	signal(SIGPIPE,SIG_IGN);
	queue_init();
	pthread_init();
	tcp_init();
	epoll_init();
	int ret;
	struct epoll_event *event;
	event=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(M+1));
	struct sockaddr_in addr;
	int addr_len;
	int i;
	int j;
	p_wait_queue wait_pcur;
	p_wait_queue wait_ppre;
	p_ready_queue ready_pcur;
	p_ready_queue ready_ppre;
	int temp;
	int recv_num;
	while(1)
	{
		memset(event,0,sizeof(struct epoll_event)*(M+1));
		addr_len=sizeof(addr);
		memset(&addr,0,addr_len);
		ret=epoll_wait(epfd,event,M+1,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(event[i].events==EPOLLIN&&event[i].data.fd==sfd)
				{
					new_fd=accept(sfd,(struct sockaddr*)&addr,&addr_len);
					if(-1==new_fd)
					{
						perror("accept");
						exit(-1);
					}
					printf("IP:%s\tPORT:%d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
					epoll_add(epfd,new_fd);
					wait_enqueue(new_fd);
				}else{
					pthread_mutex_lock(&mutex_ready);
					wait_ppre=wait_phead;
					wait_pcur=wait_phead->pnext;
					ready_ppre=ready_phead;
					ready_pcur=ready_phead->pnext;
					while(wait_pcur)
					{
						if(event[i].events==EPOLLIN&&event[i].data.fd==wait_pcur->sfd)
						{
							recv_num=recv(wait_pcur->sfd,&temp,sizeof(temp),0);
							if(recv_num>0)
							{
								printf("recv client =%d\n",temp);
								wait_to_ready(&wait_pcur,wait_ppre);
								break;
							}else{
								epoll_delete(epfd,wait_pcur->sfd);
								wait_queue_delete(&wait_pcur,wait_ppre);
								break;
							}
						}
						wait_ppre=wait_ppre->pnext;
						wait_pcur=wait_pcur->pnext;
					}
					printf("3\n");
					while(ready_pcur)
					{
						if(event[i].events==EPOLLIN&&event[i].data.fd==ready_pcur->sfd)
						{
							recv_num=recv(ready_pcur->sfd,&temp,sizeof(temp),0);
							if(recv_num>0)
							{
								printf("recv client =%d\n",temp);
								break;
							}else{
								epoll_delete(epfd,ready_pcur->sfd);
								ready_queue_delete(&ready_pcur,ready_ppre);
								break;
							}
						}
						ready_ppre=ready_ppre->pnext;
						ready_pcur=ready_pcur->pnext;
					}
					pthread_mutex_unlock(&mutex_ready);



				}
			}
		}
	}
}

