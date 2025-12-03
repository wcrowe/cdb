#ifndef FILE_H
#define FILE_H

#include "common.h"          // ← ONLY place structs are defined

int create_db_file(char *filename);
int open_db_file(char *filename, struct dbheader_t **headerOut);

#endif