#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "file.h"

int create_db_file(char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd != -1) {
    printf("File already exists.\n");
    close(fd);
    return STATUS_ERROR;
  }
  fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1) {
    perror("Error creating file");
    return STATUS_ERROR;
  }
  return STATUS_SUCCESS;
}

int open_db_file(char *filename) {
  int fd = open(filename, O_RDONLY | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1) {
    printf("File not found.\n");
    return STATUS_ERROR;
  }
  return STATUS_SUCCESS;
}
