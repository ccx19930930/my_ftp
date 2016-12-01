#include"md5.h"

int get_md5()
{
	char md5_str[MD5_STR_LEN + 1];
	char *file_path = "temp";
	md5(file_path,md5_str);
	printf("[file - %s] md5 value:\n", file_path);
	printf("%s\n", md5_str);
}
