#include <errno.h>
#include <sqlite3.h>
#include <stockage.h>
#include <cwalk.h>
#include "ospl.h"

static int create_folder(char *path)
{
	if (folder_exists(path))
	{
		errno = EEXIST;
		return 1;
	}
	else
		return create_directory(path);
}

static int create_database_file(char *path)
{
	int len = strlen(path) + DATABASE_NAME_LEN;
	char *dbpath = calloc(len, sizeof(char) + 1);
	cwk_path_join(path, DATABASE_FILENAME, dbpath, len);
	create_database(dbpath);
}

int ospl_create_library(char *path)
{
	int r;

	r = create_folder(path);
	if (r)
		return r;
	r = create_database_file(path);
	return r;
}
