#include"func.h"
#define IP "192.168.1.114"
#define PORT 2000

int main()
{
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	int ret;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=inet_addr(IP);
	ret=connect(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect success!\n");
	int temp;
	printf("input:\n");
	scanf("%d",&temp);
	send(sfd,(void*)&temp,sizeof(int),0);
	int buf;
	while(1)
	{
		ret=recv(sfd,(void*)&buf,sizeof(buf),0);
		if(-1==ret)
		{
			perror("recv");
			return -1;
		}
		if(0!=buf)
		{
			printf("queue:%d\n",buf);
		}else{
			break;
		}
	}
	printf("login\n");
	printf("logout\n");
	return 0;
}

