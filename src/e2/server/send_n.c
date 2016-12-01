#include"func.h"

int send_n(int sfd,char *p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,p+total,len-total,0);
		if(-1==ret)
		{
			perror("send");
			return -1;
		}
		total+=ret;
	}
}

int send_protocol(int sfd,data *d)
{
	printf("sfd=%d\n",sfd);
	int ret;
	ret=send_n(sfd,(void*)&(d->len),4);
	if(-1==ret)
	{
		return -1;
	}
	printf("send len =%d\n",d->len);
	ret=send_n(sfd,d->buf,d->len);
	if(-1==ret)
	{
		return -1;
	}
	printf("send buf=%s\n",d->buf);
}
