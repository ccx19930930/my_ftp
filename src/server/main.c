#include"func.h"

int main(int argc,char *argv[])
{
	if(2!=argc)
	{
		printf("error argc!\n");
		return -1;
	}
	printf("conf = %s\n",argv[1]);
	conf_struct conf;
	get_conf(&conf,argv[1]);
	printf("port=%d\nlisten_num=%d\npthread_num=%d\n",conf.port,conf.listen_num,conf.pthread_num);
	printf("download_num=%d\nupload=%d\n",conf.download_num,conf.upload_num);
	int ret=chdir("files");
	if(-1==ret)
	{
		perror("chdir");
		return -1;
	}
	factory(&conf);
}
