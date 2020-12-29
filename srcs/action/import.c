/*	libospl - Open Source Photo Library
	an opensource and multiplateform photo library management that can be used
	to store and sort all your pictures.
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

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include <cwalk.h>
#include <thumbnailer.h>
#include <stockage.h>
#include <md5.h>

#include "ospl.h"

void get_time(t_current_time *ct)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	struct timespec spec;
	
	clock_gettime(CLOCK_REALTIME, &spec);

	ct->Y = tm.tm_year + 1900;
	ct->M = tm.tm_mon + 1;
	ct->d = tm.tm_mday;
	ct->h = tm.tm_hour;
	ct->m = tm.tm_min;
	ct->s = tm.tm_sec;
	ct->ms = round(spec.tv_nsec / 1.0e6);
}

static int get_info(t_db *db, t_photos *pic, char *path, char *library)
{
	size_t len;
	char hash[33] = {0};
	t_current_time ct;
	const char *name;

	if(md5(path, hash))
		return EHASHFAIL;
	get_time(&ct);
	srand(time(0) + ct.ms);
	cwk_path_get_basename(path, &name, &len);
	sprintf(pic->random, "%010d", rand());
	strcpy(pic->hash, hash);
	strcpy(pic->original_name, name);
	sprintf(pic->import_datetime, "%d-%02d-%02d-%02d-%02d-%02d-%03d",
	ct.Y, ct.M, ct.d, ct.h, ct.m, ct.s, ct.ms);
	sprintf(pic->new_name, "%s_%s", pic->import_datetime, pic->original_name);
	pic->import_year = ct.Y;
	pic->import_month = ct.M;
	pic->import_day = ct.d;
	pic->import_hour = ct.h;
	pic->import_minut = ct.m;
	pic->import_second = ct.s;
	fill_tdb(db, library);
	return SUCCESS;
}

/**
  * \brief Import a picture into the library
  *
  * \param library library path
  * \param path picture path
  * \return 0 with success and 1 when an error occurs
  */
int ospl_import_picture(char *library, char *path)
{
	t_db db = {0};
	t_photos pic = {0};
	int r = 0;
	if (!file_exists(path))
		return ENOTFOUND;
	if(!is_supported(path))
		return ENOSUPPORT;
	if (get_info(&db, &pic, path, library))
		return EHASHFAIL;
	#ifdef DEBUG
		printf("\t----------------DEBUGGING-INFO-----------\n");
		printf("library: %s\npath: %s\n\n", library, path);
		printf("hash:\t\t\t|%s\n", pic.hash);
		printf("original_name:\t\t|%s\n", pic.original_name);
		printf("new_name:\t\t|%s\n", pic.new_name);
		printf("import_datetime:\t|%s\n", pic.import_datetime);
		printf("random:\t\t\t|%s\n", pic.random);
		printf("import_year:\t\t|%d\n", pic.import_year);
		printf("import_month:\t\t|%d\n", pic.import_month);
		printf("import_day:\t\t|%d\n", pic.import_day);
		printf("import_hour:\t\t|%d\n", pic.import_hour);
		printf("import_minut:\t\t|%d\n", pic.import_minut);
		printf("import_second:\t\t|%d\n", pic.import_second);
		printf("exif_height:\t\t|%d\n", pic.exif_height);
		printf("exif_width:\t\t|%d\n", pic.exif_width);
		printf("exif_brand:\t\t|%s\n", pic.exif_brand);
		printf("exif_peripheral:\t|%s\n", pic.exif_peripheral);
		printf("fav:\t\t\t|%d\n", pic.fav);
		printf("\t------------END DEBUGGING----------------\n");
	#endif
	printf("adding %s to database\n", pic.original_name);
	r = insert_photo(&db, &pic);
	if (r)
		return EDBFAIL;
	char import_path[512] = {0};
	char thumb_path[512] = {0};
	cwk_path_join(library, "/pictures/import", import_path, sizeof(import_path));
	cwk_path_join(library, "/thumbnails/", thumb_path, sizeof(thumb_path));
	cwk_path_join(thumb_path, pic.new_name, thumb_path, sizeof(thumb_path));
	cwk_path_join(import_path, pic.new_name, import_path, sizeof(import_path));
	printf("copying %s to %s\n", path, import_path);
	r = copy_file(path, import_path);
	if (r < 0)
		return EERRNO;
	printf("copied %d bytes\n", r);
	printf("creating thumbnail from %s to %s\n", import_path, thumb_path);
	r = create_thumbnail(import_path, thumb_path, THUMB_HEIGHT);
	if (r)
		return ETHUMBFAIL;
	printf("picture %s imported\n", pic.original_name);
	return SUCCESS;
}

int ospl_import_folder(char *library, char *path)
{
	DIR *d;
	struct dirent *dir;

	printf("folder to import: %s\n", path);
	if (!(d = opendir(path)))
		return ENOTFOUND;
	char tmp[4096] = { 0 };
	readdir(d);
	readdir(d);
	while ((dir = readdir(d)))
	{
		cwk_path_join(path, dir->d_name, tmp, sizeof(tmp));
		ospl_import_picture(library, tmp);
	}
	closedir(d);
	return SUCCESS;
}
