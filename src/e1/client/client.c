#include"func.h"

void client(int sfd)
{
	char buf[1024];
	int buf_len;
	int temp;
	while(1)
	{
		printf("input 1-7\n");
		scanf("%d",&temp);
		send(sfd,(void*)&temp,sizeof(temp),0);
		switch(temp)
		{
			case 1:
				{
					memset(buf,0,sizeof(buf));
					recv_n(sfd,(char*)&buf_len,4);
					recv_n(sfd,buf,buf_len);
					printf("pdw=%s\n",buf);
					break;
				}

			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:printf("input %d\n",temp);break;
		}
	}
	close(sfd);
}
