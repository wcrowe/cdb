#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {
  return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
  return STATUS_SUCCESS;
}

int output_file(int fd, struct dbheader_t *dbhdr) {  //, struct employee_t *employees) {
  if (fd < 0) {
    printf("Bad fd to validate_db_header.\n");
    return STATUS_ERROR;
  }
  dbhdr->magic = htonl(dbhdr->magic);
  dbhdr->version = htons(dbhdr->version);
  dbhdr->count = htons(dbhdr->count);
  dbhdr->filesize = htonl(dbhdr->filesize);
  lseek(fd, 0, SEEK_SET);
  ssize_t written_bytes = write(fd, dbhdr, sizeof(struct dbheader_t));
  if (written_bytes != sizeof(struct dbheader_t)) {
    printf("Failed to write header to file.\n");
    return STATUS_ERROR;
  }
  printf("Database header written successfully.\n");
  // *employeesOut = employees;
  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
  if (fd < 0) {
    printf("Bad fd to validate_db_header.\n");
    return STATUS_ERROR;
  }
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Failed to allocate memory for header.\n");
    return STATUS_ERROR;
  }

  ssize_t read_bytes = read(fd, header, sizeof(struct dbheader_t));
  if (read_bytes != sizeof(struct dbheader_t)) {
    perror("Failed to read header from file");
    free(header);
    return STATUS_ERROR;
  }
  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  if (header->version != 0x1) {
    printf("Unsupported version in header.\n");
    free(header);
    return STATUS_ERROR;
  }
  if (header->magic != HEADER_MAGIC) {
    printf("Invalid magic number in header.\n");
    free(header);
    return STATUS_ERROR;
  }
  struct stat sb = {0};
  if (fstat(fd, &sb) == -1) {
    printf("Failed to stat file.\n");
    free(header);
    return STATUS_ERROR;
  }
  if (header->filesize != (unsigned int)sb.st_size) {
    printf("File size mismatch in header.\n");
    free(header);
    return STATUS_ERROR;
  }
  *headerOut = header;

  return STATUS_SUCCESS;
}

int create_db_header(int fd, struct dbheader_t **headerOut) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed to create header.\n");
    return STATUS_ERROR;
  }
  header->magic = HEADER_MAGIC;
  header->version = 0x1;
  header->count = 0;
  header->filesize = sizeof(struct dbheader_t);
  *headerOut = header;
  return STATUS_SUCCESS;
}
