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
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sqlite3.h>
#include "stockage.h"


/**
 * \brief Checks sqlite return
 *
 * Checks whether db or query execution is OK or not. 
 * If not,prints the error message to stderr and returns error code 1
 * \param rc return code to check
 * \param db database data structur
 * \return 0 if success, or the sqlite error code if failed
 */
static int check_sqlite_return(int rc, t_db *db, char *query)
{
	if (rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE)
	{
		fprintf(stderr, "Error %d: %s | query: {%s}\n", rc, sqlite3_errmsg(db->db),
				query);
		sqlite3_close(db->db);
		return rc;
	}
	return 0;
}

int stockage_get_last_insert_rowid(sqlite3 *sqlite)
{
	return sqlite3_last_insert_rowid(sqlite);
}

int stockage_create_db(char *path)
{
	sqlite3 		*db;
	int				rc;
	char			*vacuum = "PRAGMA auto_vacuum = FULL;";

	if (!path)
		return -1;
	rc = sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE | 
							SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE, NULL);
	if (rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE)
	{
		fprintf(stderr, "Error %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}
	rc = sqlite3_exec(db, vacuum, 0, 0, NULL);
	if (rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE)
	{
		fprintf(stderr, "Error %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}
	rc = sqlite3_exec(db, TABLE, 0, 0, NULL);
	if (rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE)
	{
		fprintf(stderr, "Error %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}
	sqlite3_close(db);
	return 0;
}

int stockage_init(t_db *db)
{
	int rc;

	rc = sqlite3_open_v2(db->path, &db->db, SQLITE_OPEN_READWRITE | 
							SQLITE_OPEN_FULLMUTEX, NULL);
	check_sqlite_return(rc, db, "empty");
	rc = sqlite3_exec(db->db, JOURNAL_MODE_MEMORY, 0, 0, NULL);
	check_sqlite_return(rc, db, "empty");
	rc = sqlite3_exec(db->db, SYNCRONOUS_OFF, 0, 0, NULL);
	check_sqlite_return(rc, db, "empty");
	rc = sqlite3_exec(db->db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	return check_sqlite_return(rc, db, "BEGIN TRANSACTION");
}

int stockage_query_write(char *query, t_db *db)
{
	int rc;

	rc = sqlite3_exec(db->db, query, 0, 0, NULL);
	return check_sqlite_return(rc, db, query);
}

int stockage_query_read(char *query, t_db *db, int callback(), void *data)
{
	int rc;

	rc = sqlite3_exec(db->db, query, callback, data, NULL);
	return check_sqlite_return(rc, db, query);
}

int stockage_commit(t_db *db)
{
	int rc;

	rc = sqlite3_exec(db->db, "END TRANSACTION", NULL, NULL, NULL);
	check_sqlite_return(rc, db, "END TRANSACTION");
	rc = sqlite3_close_v2(db->db);
	return check_sqlite_return(rc, db, "empty");
}

int stockage_read(t_db *db, char *query, int callback(), void *value)
{
	if (check_sqlite_return(stockage_init(db), db, query))
		return -1;
	if (check_sqlite_return(stockage_query_read(query, db, callback, value), db, query))
		return -1;
	if (check_sqlite_return(stockage_commit(db), db, query))
		return -1;
	return 0;
}

int stockage_write(t_db *db, char *query)
{
	if (check_sqlite_return(stockage_init(db), db, query))
		return -1;
	if (check_sqlite_return(stockage_query_write(query, db), db, query))
		return -1;
	if (check_sqlite_return(stockage_commit(db), db, query))
		return -1;
	return 0;
}
