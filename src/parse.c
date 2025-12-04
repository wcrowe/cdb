#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {
  // Work on a copy — NEVER modify optarg/argv
  char buffer[1024];
  if (strlen(addstring) >= sizeof(buffer)) {
    printf("Input string too long\n");
    return STATUS_ERROR;
  }
  strcpy(buffer, addstring);

  char *name = strtok(buffer, ",");
  char *addr = strtok(NULL, ",");
  char *hours_str = strtok(NULL, ",");

  if (name == NULL || addr == NULL || hours_str == NULL) {
    printf("Invalid format: expected name,address,hours\n");
    return STATUS_ERROR;
  }

  int idx = dbhdr->count - 1;

  strncpy(employees[idx].name, name, sizeof(employees[idx].name) - 1);
  employees[idx].name[sizeof(employees[idx].name) - 1] = '\0';

  strncpy(employees[idx].address, addr, sizeof(employees[idx].address) - 1);
  employees[idx].address[sizeof(employees[idx].address) - 1] = '\0';

  employees[idx].hours = atoi(hours_str);

  return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
  if (fd < 0) {
    printf("Got a bad FD from the user\n");
    return STATUS_ERROR;
  }

  int count = dbhdr->count;

  struct employee_t *employees = calloc(count, sizeof(struct employee_t));
  if (employees == NULL) {
    printf("Malloc failed\n");
    return STATUS_ERROR;
  }

  read(fd, employees, count * sizeof(struct employee_t));

  int i = 0;
  for (; i < count; i++) {
    employees[i].hours = ntohl(employees[i].hours);
  }

  *employeesOut = employees;
  return STATUS_SUCCESS;
}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
  if (fd < 0) {
    printf("Got a bad FD from the user\n");
    return STATUS_ERROR;
  }

  int realcount = dbhdr->count;

  dbhdr->magic = htonl(dbhdr->magic);
  dbhdr->filesize = htonl(sizeof(struct dbheader_t) + (sizeof(struct employee_t) * realcount));
  dbhdr->count = htons(dbhdr->count);
  dbhdr->version = htons(dbhdr->version);

  lseek(fd, 0, SEEK_SET);

  write(fd, dbhdr, sizeof(struct dbheader_t));

  int i = 0;
  for (; i < realcount; i++) {
    employees[i].hours = htonl(employees[i].hours);
    write(fd, &employees[i], sizeof(struct employee_t));
  }

  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
  printf("Validating DB header...\n");
  if (fd < 0) {
    printf("Got a bad FD from the user\n");
    return STATUS_ERROR;
  }

  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed create a db header\n");
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
    perror("read");
    free(header);
    return STATUS_ERROR;
  }
  printf("Read DB header from file\n");
  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  if (header->magic != HEADER_MAGIC) {
    printf("Impromper header magic\n");
    free(header);
    return -1;
  }
  printf("Validated DB header\n");

  if (header->version != 1) {
    printf("Impromper header version\n");
    free(header);
    return -1;
  }
  struct stat dbstat = {0};
  fstat(fd, &dbstat);
  if (header->filesize != dbstat.st_size) {
    printf("Corrupted database\n");
    free(header);
    return -1;
  }

  *headerOut = header;
  return STATUS_SUCCESS;
}

int create_db_header(struct dbheader_t **headerOut) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed to create db header\n");
    return STATUS_ERROR;
  }

  header->version = 1;
  header->count = 0;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);

  *headerOut = header;
  return STATUS_SUCCESS;
}
void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {
  if (dbhdr == NULL || employees == NULL) {
    printf("No database loaded.\n");
    return;
  }

  printf("\n=== Employee Database (%d record%s) ===\n\n", dbhdr->count,
         dbhdr->count == 1 ? "" : "s");

  if (dbhdr->count == 0) {
    printf("  (empty)\n\n");
    return;
  }

  for (int i = 0; i < dbhdr->count; i++) {
    printf("Employee #%d\n", i + 1);
    printf("  Name   : %s\n", employees[i].name);
    printf("  Address: %s\n", employees[i].address);
    printf("  Hours  : %u\n\n", employees[i].hours);
  }
}