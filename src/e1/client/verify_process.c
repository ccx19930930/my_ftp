#include"func.h"

void verify_process(int sfd)
{
	int ret;
	int temp;
	char buf;
lable:
	printf("please input your user and password\nlike \"user password\"");
	scanf("%c",&buf);
	ret=send(sfd,&buf,sizeof(buf),0);
	if(-1==ret)
	{
		perror("send");
		exit(-1);
	}
	printf("send success!\n");
	ret=recv(sfd,&temp,sizeof(temp),0);
	if(-1==ret)
	{
		perror("recv");
		exit(-1);
	}
	if(0==temp)
	{
		printf("verify success!\n");
		return ;
	}
	printf("verify default!\n");
	goto lable;
}
