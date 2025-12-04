#ifndef PARSE_H
#define PARSE_H

#include "common.h"

#define HEADER_MAGIC 0x4c4c4144
#define NAME_LEN 256
#define ADDRESS_LEN 256

// These are the EXACT signatures the tester expects
int create_db_header(struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut);
int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees);
void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees);
int add_employee(struct dbheader_t *dbhdr, struct employee_t **employees, char *addstring);  // ← note **employees

#endif