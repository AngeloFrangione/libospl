/*	libospl - Open Source Photo Library
	an opensource and multiplateform photo library management that can be used
	to store and sort all your photos.
	Copyright (C) 2019-2020 Angelo Frangione

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "includes/ospl.h"
#include "includes/ospl_cli.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

char *ARGS[ARGS_COUNT] = {"create", "import", "photo", "export", "album", "folder"};
char *ALBUM_ARGS[ALB_ARG_CNT] = {"list", "create", "rename", "delete", "phoadd", "phodel", "pholist", "phomove"};
enum {LIST = 1, CREATE, RENAME, DELETE, PHOADD, PHODEL, PHOLIST, PHOMOVE};
char *PHOTO_ARGS[PHO_ARG_CNT] = {"list", "info", "delete"};
char *g_library_path = NULL;
char *g_database_path = NULL;
int print_mode = 0; // 0 = normal printing, 1 = verbose printing, 2 = json printing

int isnumeric(char *s)
{
	while (*s)
	{
		if (!isdigit(*s))
			return 0;
		++s;
	}
	return 1;
}

void show_usage()
{
	printf("\nOSPL - Open source photo library v%s.%s.%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION);
	printf("Utility to manage an OSPL\n");
	printf("================================================================================\n");
	printf("Usage: ospl <verb> <librarypath> <options>, where <verb> is as follows:\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("ospl create         creating library\n");
	printf("ospl import         importing photos\n");
	printf("ospl photo          managing photos\n");
	printf("ospl export         exporting photos\n");
	printf("ospl album          managing albums\n");
	printf("ospl folder         managing folders\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("ospl <verb> with no options will provide help on that verb\n");
}

void usage_create()
{
	printf("\nCreates a shiny new ospl library at <librarypath>\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("Usage: ospl create <librarypath> [options]\n");
	printf("Options that are not mandatory (only one of these):\n");
	printf("-v verbose printing\n");
}

void usage_import()
{
	printf("\nImport specified photos into <librarypath>\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("Usage: ospl import <librarypath> <options>\n");
	printf("OPTIONS:\n");
	printf("At least one of the following :\n");
	printf("    -f <path> <optionnal album>: specifies a file to import into optionnal album\n");
	printf("    -F <path> <optionnal album>: specifies a folder to import into optionnal album\n");
	printf("Options that are not mandatory :\n");
	printf("    -v : human readable maximum verbosity\n");
	printf("//  -a <album_id> : import into album_id album (not implemented yet)\n");
}

void usage_export(char *arg)
{

}

void usage_album(char *arg)
{
	printf("\nManages albums at <librarypath>\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("Usage: ospl album <verb> <librarypath> <options>, where <verb> is as follows:\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("ospl album list    <librarypath>                   list albums\n");
	printf("ospl album create  <librarypath> <name>            creating albums\n");
	printf("ospl album rename  <librarypath> <album> <name>    renaming albums\n");
	printf("ospl album delete  <librarypath> <album>           deleting albums\n");
	printf("ospl album phoadd  <librarypath> <album> <photo>   add a photo into album\n");
	printf("ospl album phodel  <librarypath> <album> <photo>   delete a photo from album\n");
	printf("ospl album pholist <librarypath> <album>           list photos inside an album\n");
	printf("ospl album phomove <librarypath> <photo> <old> <new> move photo to another album\n");
	printf("--------------------------------------------------------------------------------\n");
}

void usage_folder(char *arg)
{

}

void usage_photo(char **arg)
{
	printf("\nManages photos at <librarypath>\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("Usage: ospl photo <verb> <librarypath> <options>, where <verb> is as follows:\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("ospl photo list   <librarypath>             list every photo\n");
	printf("ospl photo info   <librarypath> <id>        show more info about a photo\n");
	printf("ospl photo delete <librarypath> <id>        remove photo from library\n");
	printf("--------------------------------------------------------------------------------\n");

}
/**
  * \brief Returns the index of given arg in available arguments
  *
  * \param arg Argument to get index of
  * \return The index of arg in available arguments
  */
static int indexof_arg(char *arg, char **args, int count)
{
	for (int i = 0; i < count; i++)
		if (!strcmp(args[i], arg))
			return (i + 1);
	return 0;
}

/**
  * \brief Initiate the cli global variables
  *
  * \param av library path
  * \return 0 with success and 1 when an error occurs
  */
static int initiate_path(char *path)
{
	int path_len = strlen(path);

	g_library_path = calloc(sizeof(char), (path_len + 2));
	if (!g_library_path)
		return 1;
	g_database_path = calloc(sizeof(char), (path_len + DATABASE_NAME_LEN + 1));
	if (!g_database_path)
		return 1;
	strcat(g_library_path, path);
	if (g_library_path[path_len - 1] != '/')
		strcat(g_library_path, "/");
	strcat(g_database_path, g_library_path);
	strcat(g_database_path, DATABASE_FILENAME);
	return 0;
}

void usage_unrecognized(char *arg)
{
	printf("ospl: did not recognize verb '%s' type \"ospl\" for a list\n", arg);
}

static int return_1()
{
	return 1;
}

static int import(int ac, char **av)
{
	printf("import/ (args: %d) 1: %s 2: %s 3: %s\n", ac, av[0], av[1], av[2]);
	if (ac < 5)
	{
		usage_import();
		return 0;
	}
	else if (!strcmp(av[1], "-f") && ac < 6)
	{
		initiate_path(av[0]);
		if (library_exists(av[0]))
		{
			if (ospl_import_photo(av[0], av[2]) >= 0)
				printf("photo imported with success\n");
			else
				printf("failed importing photo\n");
		}
		else
			printf("Library not found: %s\n", av[0]);
	}
	else if (!strcmp(av[1], "-F") && ac < 6)
	{
		initiate_path(av[0]);
		if (library_exists(av[0]))
			ospl_import_folder(av[0], av[2]);
		else
			printf("Library not found: %s\n", av[0]);
	}
	else if (!strcmp(av[1], "-f") && ac < 7)
	{
		initiate_path(av[0]);
		if (library_exists(av[0]))
		{
			if (ospl_import_photo_in_album(av[0], av[2], 1) >= 0)
				printf("photo imported with success\n");
			else
				printf("failed importing photo\n");
		}
		else
			printf("Library not found: %s\n", av[0]);
	}
	else if (!strcmp(av[1], "-F") && ac < 7)
	{
		initiate_path(av[0]);
		if (library_exists(av[0]))
			ospl_import_folder_in_album(av[0], av[2], 1);
		else
			printf("Library not found: %s\n", av[0]);
	}
	return 0;
}

static int export(int ac, char **av)
{
	printf("export/ (args: %d) 1: %s 2: %s\n", ac, av[0], av[1]);
	return 0;
}

static int album (int ac, char **av)
{
	int r, ret;
	printf("album/ (args: %d) 0: %s 1: %s\n", ac, av[0], av[1]);
	if (ac < 4)
	{
		usage_album(NULL);
		return 0;
	}
	else if ((r = indexof_arg(av[0], ALBUM_ARGS, ALB_ARG_CNT)))
	{
		if(!library_exists(av[1]))
		{
			printf("Library not found: %s\n", av[1]);
			return 0;
		}
		switch(r)
		{
			case LIST :
				printf("Album list:\n=================\n");
				printf("%3s | %10s\n", "id", "name");
				printf("----------------\n");
				t_album alb_list[ALB_LIMITS] = { 0 };
				ospl_album_list(av[1], alb_list);
				for(int i = 0; alb_list[i].id; i++)
					printf("%3d | %10s\n", alb_list[i].id, alb_list[i].name);
				break;
			case CREATE :
				if (ac < 5)
				{
					usage_album(NULL);
					return 0;
				}
				ret = ospl_album_create(av[1], av[2]);
				if (ret >= SUCCESS)
					printf("album %s created\n", av[2]);
				else
					printf("error while creating album: %d\n", ret);
				break;
			case RENAME :
				if (ac < 6 || !isnumeric(av[2]))
				{
					usage_album(NULL);
					return 0;
				}
				ret = ospl_album_rename(av[1], atoi(av[2]), av[3]);
				if (ret >= SUCCESS)
					printf("album %d renamed to %s\n", atoi(av[2]), av[3]);
				else
					printf("error while creating album: %d\n", ret);
				break;
			case DELETE :
				if (ac < 5 || !isnumeric(av[2]))
				{
					usage_album(NULL);
					return 0;
				}
				ret = ospl_album_delete(av[1], atoi(av[2]));
				if (ret < 0)
					printf("error deleting album: %s\n", ospl_enum_error(ret));
				else
					printf("deleted album %s\n", av[2]);
				break;
			case PHOADD :
				if (ac < 6 || !isnumeric(av[2]) || !isnumeric(av[3]))
				{
					usage_album(NULL);
					return 0;
				}
				ospl_album_add_photo(av[1], atoi(av[2]), atoi(av[3]));
				printf("photo %d added to album %d\n", atoi(av[2]), atoi(av[3]));
				break;
			case PHODEL :
				if (ac < 6 || !isnumeric(av[2]) || !isnumeric(av[3]))
				{
					usage_album(NULL);
					return 0;
				}
				ospl_album_delete_photo(av[1], atoi(av[2]), atoi(av[3]));
				printf("photo %d deleted from album %d\n", atoi(av[2]), atoi(av[3]));
				break;
			case PHOLIST :
				if (ac < 5 || !isnumeric(av[2]))
				{
					usage_album(NULL);
					return 0;
				}
				t_photos pho_list[6500] = { 0 };
				ospl_album_list_photos(av[1], atoi(av[2]), pho_list);
				printf("Photo inside album %d:\n", atoi(av[2]));
				printf("----------------------\n");
				// printf("%d\n", pho_list[0]);
				int i = 0;
				while(pho_list[i].id)
				{
					printf("%s : %s\n", pho_list[i].original_name, pho_list[i].new_name);
					++i;
				}
				break;
			case PHOMOVE :
				if (ac < 7 || !isnumeric(av[2]) || !isnumeric(av[3]) || !isnumeric(av[4]))
				{
					usage_album(NULL);
					return 0;
				}
				ospl_album_move_photo(av[1], atoi(av[2]), atoi(av[3]), atoi(av[4]));
				printf("photo %d moved from album %d to album %d\n", atoi(av[2]), atoi(av[3]), atoi(av[4]));
				break;
		}
	}
	else
		usage_album(NULL);
	return 0;
}

static int create(int ac, char **av)
{
	initiate_path(av[0]);
	if (av[1] && !strcmp(av[1], "-v"))
	{
		printf("g_library_path: {%s}\ng_database_path: {%s}\n",
				g_library_path, g_database_path);
	}
	if (ospl_create_library(g_library_path) < 0)
		perror(NULL);
	else
		printf("created library at path: %s\n", av[0]);
	return 0;
}

static int photo(int ac, char **av)
{
	int r;
	
	printf("photo/ (args: %d) 1: %s 2: %s\n", ac, av[1], av[2]);
	if (ac < 4)
	{
		usage_photo(NULL);
		return 0;
	}
	else if ((r = indexof_arg(av[0], PHOTO_ARGS, PHO_ARG_CNT)))
	{
		if(!library_exists(av[1]))
		{
			printf("Library not found: %s\n", av[1]);
			return 0;
		}
		switch(r)
		{
			case 1 :
				printf("list\n");
				t_photos pho_list[6500] = { 0 };
				ospl_photo_list(av[1], pho_list);
				printf("%5s|%32s|%52s|%24s|%11s|%10s\n",
					"id", "hash", "original_name", "import_datetime",
					"exif_height", "exif_width");
				for(int i = 0; pho_list[i].import_year; i++)
					printf("%5d|%32s|%52s|%24s|%11d|%10d\n",
						pho_list[i].id, pho_list[i].hash,
						pho_list[i].original_name, pho_list[i].import_datetime,
						pho_list[i].exif_height, pho_list[i].exif_width);
				break;
				break;
			case 2:
				printf("info\n");
				if (ac < 5 || !isnumeric(av[2]))
				{
					usage_photo(NULL);
					return 0;
				}
				t_photos pho = { 0 };
				ospl_photo_get(av[1], atoi(av[2]), &pho);
				printf("%5s|%32s|%52s|%24s|%11s|%10s\n",
					"id", "hash", "original_name", "import_datetime",
					"exif_height", "exif_width");
				printf("%5d|%32s|%52s|%24s|%11d|%10d\n",
					pho.id, pho.hash, pho.original_name, pho.import_datetime,
					pho.exif_height, pho.exif_width);
				break;
			case 3:
				if (ac < 5 || !isnumeric(av[2]))
				{
					usage_photo(NULL);
					return 0;
				}
				ospl_photo_delete(av[1], atoi(av[2]));
				printf("deleted photo %s from library\n", av[2]);
				break;
		}
	}
	else
		usage_photo(NULL);
	return 0;
}
int main(int ac, char *av[])
{
if (ac == 1)
		show_usage();
	else if (ac == 2)
	{
		printf("index of %s: %d\n", av[1], indexof_arg(av[1], ARGS, ARGS_COUNT));
		void (*usage_launcher[1 + ARGS_COUNT])() = {usage_unrecognized, usage_create, usage_import, usage_photo, usage_export, usage_album, usage_folder};
		usage_launcher[indexof_arg(av[1], ARGS, ARGS_COUNT)](av[1]);
	}
	else
	{
		printf("index of %s: %d\n", av[1], indexof_arg(av[1], ARGS, ARGS_COUNT));
		void (*usage_launcher[1 + ARGS_COUNT])() = {usage_unrecognized, usage_create, usage_import, usage_photo, usage_export, usage_album};
		int (*function_launcher[1 + ARGS_COUNT])() = {return_1, create, import, photo, export, album};
		if (function_launcher[indexof_arg(av[1], ARGS, ARGS_COUNT)](ac, &av[2]) == 1)
			usage_launcher[indexof_arg(av[1], ARGS, ARGS_COUNT)](av[2]);
	}
	free(g_library_path);
	free(g_database_path);
}