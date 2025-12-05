#ifndef FILE_H
#define FILE_H

#include "common.h"

/*
 * create_db_file — Create a new empty database file
 * Returns file descriptor on success, STATUS_ERROR on failure
 * Fails if file already exists (safe against accidental overwrite)
 */
int create_db_file(const char *path);

/*
 * open_db_file — Open existing database for reading and writing
 * Returns file descriptor on success, STATUS_ERROR on failure
 */
int open_db_file(const char *path);

#endif /* FILE_H */