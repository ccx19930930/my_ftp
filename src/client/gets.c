#include"func.h"

void client_gets(int sfd,data *d)
{
	int temp;
	int ret;
	off_t seek;
	struct stat buf;
	memset(&buf,0,sizeof(buf));
	ret=stat(d->buf,&buf);
	if(-1==ret)
	{
		seek=0;
	}else{
		seek=buf.st_size;
	}
	send_protocol(sfd,d);
	recv(sfd,(void*)&temp,sizeof(temp),0);
	if(0==temp)
	{
		printf("No such file or directory\n");
		return;
	}
	if(-1==temp)
	{
		printf("File is invalid\n");
		return;
	}
	send_n(sfd,(void*)&seek,8);
	mode_t right=0666;
	if(0==seek)
	{
		recv_n(sfd,(void*)&right,2);
	}
	int fd;
	fd=open(d->buf,O_WRONLY|O_CREAT,right);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	lseek(fd,seek,SEEK_SET);
	data gets_d;
	while(1)
	{
		memset(&gets_d,0,sizeof(gets_d));
		recv_n(sfd,(char*)&gets_d.len,4);
		if(gets_d.len>0)
		{
			recv_n(sfd,gets_d.buf,gets_d.len);
			write(fd,gets_d.buf,gets_d.len);
		}else{
			close(fd);
			break;
		}
	}
	printf("gets success!\n");
}
