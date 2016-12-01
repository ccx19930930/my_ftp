#ifndef __CONF_H__
#define __CONF_H__


typedef struct
{
	int port;
	int listen_num;
	int pthread_num;
	int download_num;
	int upload_num;
}conf_struct,*pconf_struct;

void get_conf(conf_struct *conf,char *p);
#endif
