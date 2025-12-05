#ifndef FILE_H
#define FILE_H

#include "common.h"

/* Create a new database file — fails if exists (safe) */
int create_db_file(const char *path);

/* Open an existing database file for read/write */
int open_db_file(const char *path);

#endif /* FILE_H */