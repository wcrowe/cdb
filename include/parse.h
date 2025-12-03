#ifndef PARSE_H
#define PARSE_H

#include "common.h"          // ← ONLY source of truth for structs

int create_db_header(struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut);
int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees);
void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees);
int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring);

#endif