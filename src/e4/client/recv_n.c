#include"func.h"

int recv_n(int sfd,char *p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=recv(sfd,p+total,len-total,0);
		if(-1==ret)
		{
			perror("recv");
			exit(-1);
		}
		if(0==ret)
		{
			printf("0==ret");
			return -1;
		}
		total+=ret;
	}
	return 0;
}

int recv_protocol(int sfd,data *d)
{
	int ret;
	memset(d,0,sizeof(data));
	ret=recv_n(sfd,(void*)&(d->len),4);
	if(-1==ret)
	{
		return -1;
	}
	recv_n(sfd,d->buf,d->len);
	if(-1==ret)
	{
		return -1;
	}
}
