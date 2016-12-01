#include"func.h"

void main_process(conf_struct conf,int pid,int pipe_fd)
{
	int sfd;
	sfd=tcp_init(conf.port,conf.listen_num);
	int epfd;
	epfd=epoll_init(sfd);
	struct sockaddr_in addr;
	int addr_len;
	struct epoll_event *evs;
	evs=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(conf.listen_num+1));
	int new_fd;
	int i;
	int evs_num=1;
	int temp;
	int ret;
	puser u;
	u=(puser)calloc(conf.listen_num+20,sizeof(user));
	while(1)
	{
		memset(evs,0,sizeof(struct epoll_event)*(conf.listen_num+1));
		ret=epoll_wait(epfd,evs,evs_num,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(evs[i].events==EPOLLIN)
				{
					if(evs[i].data.fd==sfd)
					{
						addr_len=sizeof(addr);
						memset(&addr,0,addr_len);
						new_fd=accept(sfd,(struct sockaddr*)&addr,&addr_len);
						epoll_add(epfd,new_fd);
						u[new_fd].sfd=new_fd;
						u[new_fd].ip=addr.sin_addr;
						evs_num++;
					}else{
						temp=verify_process(&u[evs[i].data.fd]);
						switch(temp)
						{
							case -1:
								{
									epoll_delete(epfd,u[evs[i].data.fd].sfd);
									close(u[evs[i].data.fd].sfd);
									memset(u+evs[i].data.fd,0,sizeof(user));
									break;
								}
							case 0:break;
							case 1:
								   {
									   send_user(pipe_fd,u+evs[i].data.fd);
									   epoll_delete(epfd,u[evs[i].data.fd].sfd);
									   close(u[evs[i].data.fd].sfd);
									   printf("success!\n");
								   }
						}

					}
				}
			}
		}
	}
}
