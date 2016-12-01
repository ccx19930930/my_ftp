#include"func.h"

int main(int argc,char *argv[])
{
	if(3!=argc)
	{
		printf("error argc!\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		exit(-1);
	}
	printf("socket success!\n");
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(atoi(argv[2]));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	int addr_len;
	addr_len=sizeof(addr);
	ret=connect(sfd,(struct sockaddr*)&addr,addr_len);
	if(-1==ret)
	{
		perror("connect");
		close(sfd);
		return -1;
	}
	int fd;
	int buf_len;
	char buf[N];
	memset(buf,0,sizeof(buf));
	recv_n(sfd,(char*)&buf_len,4);
	recv_n(sfd,buf,buf_len);
	fd=open(buf,O_RDWR|O_CREAT,0666);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	while(1)
	{
		memset(buf,0,sizeof(buf));
		recv_n(sfd,(char*)&buf_len,4);
		if(buf_len>0)
		{
			recv_n(sfd,buf,buf_len);
			write(fd,buf,buf_len);	
		}else{
			break;
		}
	}
	close(sfd);
	close(fd);
	printf("download success!\n");
	return 0;
}
