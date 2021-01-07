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

#include <stdio.h>
#include <string.h>
#include <ospl.h>
#include <cwalk.h>
#include <stockage.h>

int ospl_photo_get(char *library, int id, t_photos *list)
{
	t_db db = {0};

	fill_tdb(&db, library);
	if (select_photo(&db, id, list))
		return EDBFAIL;
	return SUCCESS;
}

int ospl_photo_list(char *library, t_photos *list)
{
	t_db db = {0};

	fill_tdb(&db, library);
	if (select_photo_all(&db, list))
		return EDBFAIL;
	return SUCCESS;
}

int ospl_photo_delete(char *library, int id)
{
	t_db db = {0};
	t_photos pho;
	char tmp_import[4096] = { 0 };
	char tmp_thumb[4096] = { 0 };

	fill_tdb(&db, library);
	select_photo(&db, id, &pho);
	delete_photo(&db, id);

	cwk_path_join(library, "photos/import", tmp_import, sizeof(tmp_import));
	cwk_path_join(library, "thumbnails", tmp_thumb, sizeof(tmp_thumb));
	cwk_path_join(tmp_import, pho.new_name, tmp_import, sizeof(tmp_import));
	cwk_path_join(tmp_thumb, pho.new_name, tmp_thumb, sizeof(tmp_thumb));
	if (remove(tmp_import) || remove(tmp_thumb))
		return EERRNO;
	return SUCCESS;
}
