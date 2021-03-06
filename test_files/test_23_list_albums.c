#include "includes/ospl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>


int main(void)
{
	struct timeval nstime;
	char tmp[4096] = {0};
	char lib_name[50];

	gettimeofday(&nstime, NULL);
	srand(nstime.tv_usec);
	int r = rand();
	sprintf(lib_name, "test_Library_23_%d", r);
	if (ospl_create_library(lib_name) < 0)
	{
		printf("Error executing ospl_create_library\n");
		printf("lib_name: %s\n", lib_name);
		return 1;
	}
	if (ospl_album_create(lib_name, "Vacances") < 0)
	{
		printf("error executing ospl_album_create\n");
		return 1;
	}
	if (ospl_album_create(lib_name, "Vacances en Italie") < 0)
	{
		printf("error executing ospl_album_create\n");
		return 1;
	}
	sprintf(tmp, "%s/photos/Vacances", lib_name);
	if (!folder_exists(tmp))
	{
		printf("album folder not found %s\n", tmp);
		return 1;
	}
	sprintf(tmp, "%s/photos/Vacances en Italie", lib_name);
	if (!folder_exists(tmp))
	{
		printf("album folder not found %s\n", tmp);
		return 1;
	}
	t_album list[10];
	if (ospl_album_list(lib_name, list) < 0)
	{
		printf("error executing ospl_list_albums\n");
		return 1;
	}
	if (strcmp(list[0].name, "Vacances"))
	{
		printf("album not listed correctly expected \"Vacances\" got: %s\n", list[0].name);
		return 1;
	}
	if (strcmp(list[1].name, "Vacances en Italie"))
	{
		printf("album not listed correctly expected \"Vacances en Italie\" got: %s\n", list[0].name);
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
