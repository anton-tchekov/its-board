#include <string.h>
#include "m_argv.h"

int myargc;
char **myargv;

int M_CheckParm(char *check)
{
	int i;
	for(i = 1; i < myargc; i++)
	{
		if(!strcasecmp(check, myargv[i]))
		{
			return i;
		}
	}

	return 0;
}
