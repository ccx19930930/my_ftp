#include"func.h"

extern puser u;

void *server(void *v)
{
	psub_pth p=(psub_pth)v;
	pconf_struct conf=p->conf;
	int pipe_fd=p->fdr;
	printf("server pipe=%d\n",pipe_fd);
	int epfd;
	epfd=epoll_init(pipe_fd);
	int ret;
	int i;
	struct epoll_event *evs;
	int evs_len=1;
	int temp;
	int buf;
	int buf_len;
	int sign;
	file_serve_struct f_puts;
	file_serve_struct f_gets;
	f_puts.pf=pthread_puts;
	f_gets.pf=pthread_gets;
	file_serve_init(&f_puts,conf->upload_num);
	file_serve_init(&f_gets,conf->download_num);
	//	printf("phead=%p,ptail=%p\n",f_gets.f.phead,f_gets.f.ptail);




	evs=(struct epoll_event*)malloc(sizeof(struct epoll_event)*(conf->listen_num+1));
	while(1)
	{
		printf("=============================================\n");//查看方便
		memset(evs,0,sizeof(struct epoll_event)*(conf->listen_num+1));
		ret=epoll_wait(epfd,evs,evs_len,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(evs[i].events==EPOLLIN)
				{
					if(evs[i].data.fd==pipe_fd)
					{
						temp=read(pipe_fd,&buf,sizeof(buf));
						if(temp<=0)
						{
							perror("read");
							exit(-1);
						}
						epoll_add(epfd,buf);
						evs_len++;
					}else{
						temp=recv(evs[i].data.fd,(void*)&buf,sizeof(buf),0);
						if(temp<=0)
						{
							epoll_delete(epfd,evs[i].data.fd);
							memset(u+evs[i].data.fd,0,sizeof(user));
							close(evs[i].data.fd);
							continue;
						}
						switch(buf)
						{
							case 1://pwd
								{
									server_pwd(evs[i].data.fd);
									break;
								}
							case 2://cd
								{
									server_cd(evs[i].data.fd);
									break;
								}
							case 3://ls
								{
									server_ls(evs[i].data.fd);
									break;
								}
							case 4://mkdir
								{
									server_mkdir(evs[i].data.fd);
									break;
								}

							case 5://remove
								{
									server_remove(evs[i].data.fd);
									break;
								}
							case 6://gets
								{
									server_gets(evs[i].data.fd,&f_gets);
									break;
								}
							case 7://puts
								{
									sign=server_puts(evs[i].data.fd,&f_puts);
									if(1==sign)
									{
										epoll_delete(epfd,evs[i].data.fd);
										pthread_cond_signal(&f_puts.cond);
									}
									break;
								}
						//	default:printf("buf=%d\n",buf);break;
						}
					}
				}
			}
		}
	}
}
