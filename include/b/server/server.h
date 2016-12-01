#ifndef __SERVER_H__
#define __SERVER_H__

#include"head.h"


typedef struct server_Linklist
{
	int sfd;//new_fd
	char user[32];//用户名    ls pdw cd 
	char pdw[1024];//当前路径 ls pdw cd 
	short do_what;//1-7对应一期功能 0为无操作
	struct server_Linklist *pnext;
}server_Node,*server_Linklist;


typedef struct ls_server
{
	server_Linklist p;
	struct ls_server *pnext;
}ls_Node,*ls_Linklist;

typedef struct cd_server
{
	server_Linklist p;
	char cd_where[16];//cd到哪里
	struct cd_server *pnext;
}cd_Node,*cd_Linklist;

typedef struct pdw_server
{
	server_Linklist p;
	struct pdw_server *pnext;
}pdw_Node,*pdw_Linklist;

typedef struct puts_server
{
	server_Linklist p;
	char file_name[32];//上传的文件名
	int size;//文件大小
	struct puts_server *pnext;
}puts_Node,*puts_Linklist;

typedef struct gets_server
{
	server_Linklist p;
	char file_name[32];//下载的文件名
	int size;//文件大小
	struct gets_server *pnext;	
}gets_Node,*gets_Linklist;

typedef struct remove_server
{
	server_Linklist p;
	char file_name[32];//要删除的文件名
	struct remove_server *pnext;
}remove_Node,*remove_Linklist;

#endif
