#include"func.h"

puser u;


void main_process(pconf_struct conf,pmain_pth pmain)
{
	int sfd;
	sfd=tcp_init(conf->port,conf->listen_num);
	int epfd;
	epfd=epoll_init(sfd);
	struct sockaddr_in addr;
	int addr_len;
	struct epoll_event *evs;
	evs=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(conf->listen_num+1));
	int new_fd;
	int i;
	int evs_num=1;
	int temp;
	int ret;
	int j=0;
	u=(puser)calloc(conf->listen_num+20,sizeof(user));
	while(1)
	{
		memset(evs,0,sizeof(struct epoll_event)*(conf->listen_num+1));
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
						printf("temp=%d\n",temp);
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
									   u[evs[i].data.fd].fdw=pmain[j].fdw;
									   temp=write(pmain[j].fdw,&evs[i].data.fd,sizeof(int));
									   if(temp<=0)
									   {
										   perror("write");
										   exit(-1);
									   }
									   j++;
									   if(j==conf->pthread_num)
									   {
										   j=0;
									   }
									   epoll_delete(epfd,u[evs[i].data.fd].sfd);
									   printf("success!\n");
								   }
						}

					}
				}
			}
		}
	}
}
