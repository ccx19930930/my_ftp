#include"func.h"

void client(int sfd)
{
	char buf[1024];
	int buf_len;
	int temp;
	data d;
	orders o;
	unsigned long check;
	int i;
	while(1)
	{
		memset(&d,0,sizeof(d));
		memset(buf,0,sizeof(buf));
		memset(&o,0,sizeof(o));
		temp=order_ana(&o);
		printf("order=%d\n",temp);
		switch(temp)
		{
			case 1://pwd
				{
					send(sfd,(void*)&temp,sizeof(temp),0);
					recv_protocol(sfd,&d);
					printf("pwd=%s\n",d.buf);
					break;
				}

			case 2://cd
				{
					send(sfd,(void*)&temp,sizeof(temp),0);
					strcpy(d.buf,o.buf);
					d.len=strlen(d.buf);
					send_protocol(sfd,&d);
					recv(sfd,(void*)&temp,sizeof(temp),0);
					if(0==temp)
					{
						printf("No such file or directory\n");
						break;
					}else{
						printf("success!\n");
						break;
					}
				}
			case 3://ls
				{
					send(sfd,(void*)&temp,sizeof(temp),0);
					recv(sfd,(void*)&check,sizeof(check),0);
					for(i=0;i<check;i++)
					{
						memset(&d,0,sizeof(d));
						recv_protocol(sfd,&d);
						printf("%s\n",d.buf);
					}
					break;
				}
			case 4://mkdir
				{
					send(sfd,(void*)&temp,sizeof(temp),0);
					strcpy(d.buf,o.buf);
					d.len=strlen(d.buf);
					send_protocol(sfd,&d);
					recv(sfd,(void*)&temp,sizeof(temp),0);
					if(0==temp)
					{
						printf("default\n");
						break;
					}else{
						printf("success!\n");
						break;
					}
				}
			case 5://remove
				{
					send(sfd,(void*)&temp,sizeof(temp),0);
					strcpy(d.buf,o.buf);
					d.len=strlen(d.buf);
					send_protocol(sfd,&d);
					recv(sfd,(void*)&temp,sizeof(temp),0);
					if(0==temp)
					{
						printf("not such dir or file\n");
						break;
					}else{
						printf("success!\n");
						break;
					}
				}
			case 6://gets
				{
					send(sfd,(void*)&temp,sizeof(temp),0);
					strcpy(d.buf,o.buf);
					d.len=strlen(d.buf);
					client_gets(sfd,&d);
					break;
				}
			case 7://puts
				{
					strcpy(d.buf,o.buf);
					d.len=strlen(d.buf);
					client_puts(sfd,&d);
					continue;
				}
			default:printf("No such order!\n");break;
		}
	}
	close(sfd);
}
