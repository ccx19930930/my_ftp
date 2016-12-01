#include"func.h"

void client_puts(int sfd,data *d)
{
	int ret;
	char md5_str[MD5_STR_LEN + 1];
	ret=md5(d->buf,md5_str);
	if(-1==ret)
	{
		return ;
	}
	printf("md5=%s\n",md5_str);
	int temp;
	temp=7;
	send(sfd,(void*)&temp,sizeof(temp),0);
	data puts_d;
	memset(&puts_d,0,sizeof(puts_d));
	strcpy(puts_d.buf,md5_str);
	puts_d.len=32;
	struct stat buf;
	memset(&buf,0,sizeof(buf));
	stat(d->buf,&buf);
	send_protocol(sfd,&puts_d);
	off_t size;
	send_protocol(sfd,d);

	recv_n(sfd,(char*)&size,sizeof(off_t));
	if(size==buf.st_size)
	{
		temp=0;
		send(sfd,(void*)&temp,sizeof(temp),0);
		recv(sfd,(void*)&temp,sizeof(temp),0);
		if(0==temp)
		{
			printf("file exixt!\n");
			return;
		}
		printf("puts success!\n");
		return;
	}else{
		temp=1;
		send(sfd,(void*)&temp,sizeof(temp),0);
	}

	send_n(sfd,(void*)&buf.st_mode,2);
	int total;
	int fd;
	fd=open(d->buf,O_RDONLY);
	while(1)
	{
		total=0;
		recv_n(sfd,(char*)&size,sizeof(off_t));
		lseek(fd,size,SEEK_SET);
		while(total<1024*64)
		{
			memset(d,0,sizeof(data));
			d->len=read(fd,d->buf,sizeof(d->buf));
			if(d->len<=0)
			{
				d->len=-1;
				send_n(sfd,(char*)&d->len,4);
				close(fd);
				printf("success!\n");
				return;
			}
			total+=d->len;
			ret=send_n(sfd,(char*)d,d->len+4);
			if(-1==ret)
			{
				close(fd);
				printf("outline!\n");
				exit(-1);
			}
		}
		d->len=-2;
		send_n(sfd,(char*)&d->len,4);
	}
}
		
