#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

int create_db_file(char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd != -1) {
    close(fd);
    printf("File already exists.\n");
    return STATUS_ERROR;
  }
  fd = open(filename, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("Error creating file");
    return STATUS_ERROR;
  }
  printf("Database file created successfully.\n");
  return fd;
}

int open_db_file(char *filename, struct dbheader_t **headerOut) {
    int fd = open(filename, O_RDWR, 0644);
    if (fd == -1) {
        printf("File not found.\n");
        return STATUS_ERROR;
    }

    printf("Database file opened successfully.\n");

    if (validate_db_header(fd, headerOut) == STATUS_ERROR) {
        close(fd);
        printf("Invalid database file.\n");
        return STATUS_ERROR;
    }

    // Leave file pointer right after header
    lseek(fd, sizeof(struct dbheader_t), SEEK_SET);

    return fd;
}