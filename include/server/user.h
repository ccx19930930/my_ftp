//本头文件存放用户相关信息
#ifndef __USER_H__
#define __USER_H__

#include"head.h"

typedef struct user_struct
{
	int sfd;//new_fd  同时作为数组下标
	struct in_addr ip;//IP
	char user[32];//用户名
	char user_id[4];//存放数据库中users的id
	char pwd[1024];//当前路径
	char pwd_id[4];//存放数据库中files的id 
	char lastpwd[1024];//存放上一次访问的目录
	char lastid[4];//存放上一次访问的目录id
	int fdw;//管道读端
}user,*puser;


#endif
