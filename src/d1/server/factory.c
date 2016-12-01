#include"func.h"

void factory(pconf_struct conf)
{
	psub_pth psub;
	pmain_pth pmain;
	pmain=(pmain_pth)calloc(conf->pthread_num,sizeof(main_pth));
	psub=(psub_pth)calloc(conf->pthread_num,sizeof(sub_pth));
	int i;
	int fds[2];
	int ret;
	for(i=0;i<conf->pthread_num;i++)
	{
		pipe(fds);
		pmain[i].fdw=fds[1];
		psub[i].fdr=fds[0];
		psub[i].conf=conf;
		ret=pthread_create(&(pmain[i].pthid),NULL,server,(void*)&psub[i]);
		if(-1==ret)
		{
			printf("pthread_create errno=%d\n",ret);
			exit(-1);
		}
	}
	main_process(conf,pmain);
}

