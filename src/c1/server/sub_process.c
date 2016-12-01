#include"func.h"

void sub_process(conf_struct conf,int pipe_fd)
{
	pser s;
	s=(pser)calloc(conf.listen_num+20,sizeof(ser));
	int epfd;
	epfd=epoll_init(pipe_fd);
	int ret;
	int i;
	struct epoll_event *evs;
	int evs_len=1;
	ser s_temp;
	int temp;
	int buf;
	evs=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(conf.listen_num+1));
	while(1)
	{
		memset(evs,0,sizeof(struct epoll_event)*(conf.listen_num+1));
		ret=epoll_wait(epfd,evs,evs_len,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(evs[i].events==EPOLLIN)
				{
					if(evs[i].data.fd==pipe_fd)
					{
						memset(&s_temp,0,sizeof(s_temp));
						recv_user(pipe_fd,&s_temp);
						s[s_temp.sfd]=s_temp;
						epoll_add(epfd,s_temp.sfd);
						evs_len++;
						printf("recv msg\nsfd=%d\nuser=%s\nip=%s\n",s_temp.sfd,s_temp.user,s_temp.ip);
					}else{
						temp=recv(evs[i].data.fd,(void*)&buf,sizeof(buf),0);
						if(temp<=0)
						{
							epoll_delete(epfd,evs[i].data.fd);
							memset(s+evs[i].data.fd,0,sizeof(ser));
							close(evs[i].data.fd);
							continue;
						}
						switch(buf)
						{
							case 1:
							case 2:
							case 3:
							case 4:
							case 5:	
							case 6:
							default:printf("buf=%d\n",buf);break;
						}

					}
				}
			}
		}
	}
}
