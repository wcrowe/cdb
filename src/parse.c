#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "common.h"
#include "parse.h"

int create_db_header(struct dbheader_t **headerOut)
{
    struct dbheader_t *h = calloc(1, sizeof(struct dbheader_t));
    if (!h) return STATUS_ERROR;

    h->magic    = HEADER_MAGIC;
    h->version  = 1;
    h->count    = 0;
    h->filesize = sizeof(struct dbheader_t);

    *headerOut = h;
    return STATUS_SUCCESS;
}

int add_employee(struct dbheader_t *dbhdr, struct employee_t **employees, char *addstring)
{
    char buf[1024];
    strncpy(buf, addstring, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';

    char *name  = strtok(buf, ",");
    char *addr  = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");
    if (!name || !addr || !hours) return STATUS_ERROR;

    struct employee_t *new_mem = realloc(*employees, (dbhdr->count + 1) * sizeof(struct employee_t));
    if (!new_mem) return STATUS_ERROR;
    *employees = new_mem;

    struct employee_t *e = &(*employees)[dbhdr->count];
    strncpy(e->name,    name,  NAME_LEN-1);    e->name[NAME_LEN-1]    = '\0';
    strncpy(e->address, addr,  ADDRESS_LEN-1); e->address[ADDRESS_LEN-1] = '\0';
    e->hours = atoi(hours);

    dbhdr->count++;
    return STATUS_SUCCESS;
}

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees)
{
    if (!dbhdr || !employees || dbhdr->count == 0) {
        printf("Database empty or not loaded.\n");
        return;
    }
    printf("\n=== Employee Database (%d records) ===\n\n", dbhdr->count);
    for (int i = 0; i < dbhdr->count; i++) {
        printf("Employee #%d\n", i+1);
        printf("  Name   : %s\n", employees[i].name);
        printf("  Address: %s\n", employees[i].address);
        printf("  Hours  : %u\n\n", employees[i].hours);
    }
}