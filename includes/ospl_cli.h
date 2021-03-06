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

#ifndef OSPL_CLI_H
#define OSPL_CLI_H

# define ARGS_COUNT 6
# define ALB_ARG_CNT 8
# define PHO_ARG_CNT 3

# define CREATE_OPTS "v"
# define IMPORT_OPTS "vfF"
# define ALB_LIMITS 255

extern char *ARGS[ARGS_COUNT];
extern char *g_library_path;
extern char *g_database_path;
extern int  print_mode;

#endif