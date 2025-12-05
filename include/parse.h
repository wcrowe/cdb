#ifndef PARSE_H
#define PARSE_H

#include "common.h"

/* Create a new empty header in memory */
int create_db_header(struct dbheader_t **out_header);

/* Validate header from open file */
int validate_db_header(int fd, struct dbheader_t **out_header);

/* Read all employee records */
int read_employees(int fd, struct dbheader_t *hdr, struct employee_t **out_employees);

/* Write entire database to disk */
int output_file(int fd, struct dbheader_t *hdr, struct employee_t *employees);

/* Add one employee from CSV string */
int add_employee(struct dbheader_t *hdr, struct employee_t *employees, char *addstring);

/* Print all employees */
void list_employees(struct dbheader_t *hdr, struct employee_t *employees);

#endif /* PARSE_H */