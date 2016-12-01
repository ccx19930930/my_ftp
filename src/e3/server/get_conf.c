#include"func.h"

void get_conf(conf_struct *conf,char *file)
{
	int fd;
	fd=open(file,O_RDONLY);
	if(-1==fd)
	{
		perror("open conf");
		exit(-1);
	}
	char buf[128];
	int ret;
	memset(buf,0,sizeof(buf));
	ret=read(fd,buf,sizeof(buf));
	if(-1==ret)
	{
		perror("read port");
		exit(-1);
	}
	close(fd);
	char *p;
	p=buf;
	int temp[3];
	int i=0;
	while(p<(buf+128)&&(p=strchr(p,' ')))
	{
		temp[i]=atoi(p);
		i++;
		p++;
	}
	conf->port=temp[0];
	conf->listen_num=temp[1];
	conf->pthread_num=temp[2];
}
