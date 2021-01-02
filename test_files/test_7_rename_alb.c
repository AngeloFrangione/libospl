#include "includes/ospl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


int main(void)
{
	struct timeval nstime;
	char tmp[300] = {0};
	char lib_name[50];

	gettimeofday(&nstime, NULL);
	srand(nstime.tv_usec);
	int r = rand();
	sprintf(lib_name, "test_Library_7_%d", r);
	if (ospl_create_library(lib_name) < 0)
	{
		printf("Error executing ospl_create_library\n");
		printf("lib_name: %s\n", lib_name);
		return 1;
	}
	if (ospl_create_album(lib_name, "Vacances") < 0)
	{
		printf("error executing ospl_create_album\n");
		return 1;
	}
	if (ospl_rename_album(lib_name, 1, "Vacances en France") < 0)
	{
		printf("executing ospl_rename_album failed\n");
		return 1;
	}
	sprintf(tmp, "%s/pictures/Vacances en France", lib_name);
	if (!folder_exists(tmp))
	{
		printf("album folder not found: %s\n", tmp);
		return 1;
	}
	sprintf(tmp, "rm -rf %s", lib_name);
	if (system(tmp))
	{
		printf("executing the system command failed\n");
		return 1;
	}
	return 0;
}
