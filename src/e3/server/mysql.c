#include"func.h"

extern MYSQL *conn_ptr; 

void mysql()
{
	conn_ptr = mysql_init(NULL);
	if (!conn_ptr)
	{
		printf("mysql_init failed\n");
		exit(EXIT_FAILURE);
	}
	conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "china86", "ccx_ftp", 0, NULL, 0);
	if (conn_ptr)
	{
		printf("Connection success\n");
	} else {
		printf("Connection failed\n");
	}
}

unsigned long mysql_select(char *selects,data** mysql_data)
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	MYSQL_FIELD *fd;
	int res, i, j, k=0;
	res = mysql_query(conn_ptr,selects);
	if (res) 
	{
		printf("SELECT error:%s\n",mysql_error(conn_ptr));
	} else {
		res_ptr = mysql_store_result(conn_ptr);
		if(res_ptr)
		{
			*mysql_data=(data*)calloc((unsigned long)mysql_num_rows(res_ptr),sizeof(data));
			j = mysql_num_fields(res_ptr);
			while((sqlrow = mysql_fetch_row(res_ptr)))
			{
				for(i = 0; i < j; i++)
				{
					strcpy((*mysql_data)[k].buf,sqlrow[i]);
					k++;
				}
				printf("\n");
			}
		}
		mysql_free_result(res_ptr);
	}
	return k;
}

