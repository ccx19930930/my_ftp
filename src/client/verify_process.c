#include"func.h"

int verify_process(int sfd)
{
	int ret;
	unsigned long temp;
	data d[2];
	memset(d,0,sizeof(data)*2);
lable:
	printf("please input your user and password\nlike \"user password\"\n");
	scanf("%s %s",d[0].buf,d[1].buf);
	d[0].len=strlen(d[0].buf);
	d[1].len=strlen(d[1].buf);
	ret=send_protocol(sfd,&d[0]);
	if(-1==ret)
	{
		return -1;
	}
	ret=send_protocol(sfd,&d[1]);
	if(-1==ret)
	{
		return -1;
	}
	printf("send success!\n");
	recv_n(sfd,(char*)&temp,4);
	printf("check=%ld\n",temp);
	if(temp==1)
	{
		printf("verify success!\n");
		return 0;
	}
	printf("verify default!\n");
	goto lable;
}
