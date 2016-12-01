#ifndef __FILE_H__
#define __FILE_H__

#include"head.h"

typedef struct file_system
{
	int type;//文件类型  1为目录  2为文件
	int level;//第几级目录
	char name;//文件名
	int size;//文件大小  目录为0
	struct file_system *pnext;
}file_Node,*file_Linklist;





#endif
