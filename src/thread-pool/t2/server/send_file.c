#include"factory.h"

void send_file(int sfd)
{
	data d;
	int fd;
	fd=open(DOWNLOAD,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		exit(-1);
	}
	memset(&d,0,sizeof(d));
	d.len=strlen(DOWNLOAD);
	strcpy(d.buf,DOWNLOAD);
	send_n(sfd,(char*)&d,d.len+4);
	while(1)
	{
		memset(&d,0,sizeof(d));
		d.len=read(fd,d.buf,sizeof(d.buf));
		if(d.len<=0)
		{
			d.len=0;
			send_n(sfd,(char*)&d,4);
			break;
		}
		send_n(sfd,(char*)&d,d.len+4);
	}
	printf("send file success!\n");
	close(sfd);
}
