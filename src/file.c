#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"
#include "file.h"

int create_db_file(const char *path)
{
    int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd < 0) {
        perror("create_db_file");
        return STATUS_ERROR;
    }
    printf("Created new database: %s\n", path);
    return fd;
}

int open_db_file(const char *path)
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        perror("open_db_file");
        return STATUS_ERROR;
    }
    return fd;
}