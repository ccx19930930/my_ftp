#ifndef __MYSQL_H__
#define __MYSQL_H__

#include"head.h"
#include"protocol.h"
#include<mysql/mysql.h>

typedef struct
{
	int len;
	char *p;
}mysql_data,*p_mysql_data;

void mysql();
unsigned long mysql_select(char *selects,data** mysql_data);


#endif
