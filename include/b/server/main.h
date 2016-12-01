#ifndef __MAIN_H__
#define __MAIN_H__

#include"head.h"


typedef struct main_linklist//主进程中存放的信息  链表形式
{
	int sfd;
	in_addr_t ip;//网络字节序
	short port;//端口号
	short flag;//标记位 是否已登陆
	char user[32];//存放用户名
	struct main_linklist *pnext;
}main_Node,*main_linklist;



#endif
