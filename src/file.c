#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "common.h"

/*
 * Create a new database file.
 * Fails if file already exists (O_EXCL = exclusive creation).
 */
int create_db_file(const char *path)
{
    /* O_EXCL ensures atomic "create if not exists" */
    int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd < 0) {
        perror("create_db_file: open failed");
        return STATUS_ERROR;
    }
    printf("Created new database: %s\n", path);
    return fd;
}

/*
 * Open an existing database file for reading and writing.
 */
int open_db_file(const char *path)
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        perror("open_db_file: open failed");
        return STATUS_ERROR;
    }
    return fd;
}