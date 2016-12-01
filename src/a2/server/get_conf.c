#include"func.h"

void get_conf(conf_struct *conf)
{
	int fd;
	fd=open("../../conf/_server.conf",O_RDONLY);
	if(-1==fd)
	{
		perror("open conf");
		exit(-1);
	}
	char buf[128];
	int ret;
	memset(buf,0,sizeof(buf));
	ret=read(fd,buf,sizeof(buf));
	printf("ret=%d\n",ret);
	printf("buf=%s\n",buf);
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
	printf("%d %d %d\n",temp[0],temp[1],temp[2]);
	conf->port=temp[0];
	conf->pthread_num=temp[1];
	conf->download_pthread_num=temp[2];
	printf("%d %d %d\n",conf->port,conf->pthread_num,conf->download_pthread_num);
}
