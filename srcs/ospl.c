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

#include "ospl.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

char *enum_error(int error_code)
{
	switch (error_code)
	{
		case SUCCESS:
			return NULL;
			break;
		case EAEXISTS :
			return "the file already exists\n";
			break;
		case EDBFAIL :
			return "something went wrong with the database\n";
			break;
		case EERRNO :
			return strerror(errno);
			break;
		case ENOTFOUND :
			return "file not found\n";
			break;
		case ENOSUPPORT :
			return "not supported\n";
			break;
		case ETHUMBFAIL :
			return "something went wrong with the thumbnail creation\n";
			break;
		case EHASHFAIL :
			return "something went wrong with the md5 hash\n";
			break;
		default :
			return "unknown error\n";
			break;
	}
}