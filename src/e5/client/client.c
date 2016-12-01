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
		send(sfd,(void*)&temp,sizeof(temp),0);
		switch(temp)
		{
			case 1://pwd
				{
					recv_protocol(sfd,&d);
					printf("pwd=%s\n",d.buf);
					break;
				}
			case 3://ls
				{
					recv(sfd,(void*)&check,sizeof(check),0);
					for(i=0;i<check;i++)
					{
						memset(&d,0,sizeof(d));
						recv_protocol(sfd,&d);
						printf("%s\n",d.buf);
					}
					break;
				}
			default:
				{
					strcpy(d.buf,o.buf);
					d.len=strlen(d.buf);
					send_protocol(sfd,&d);
					switch(temp)
					{

						case 2://cd
							{
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
						case 4://mkdir
							{
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
						case 5:
						case 6:
						default:printf("input %d\n",temp);break;
					}
					break;
				}
		}
	}
	close(sfd);
}
