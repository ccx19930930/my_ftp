#include"func.h"

int main()
{
	factory fac;
	if(!fork())
	{
		verify_process();
	}
	if(!fork())
	{
		serve_process(&fac);
	}
	while(1);
}
