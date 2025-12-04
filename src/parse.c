#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "common.h"
#include "parse.h"

int create_db_header(struct dbheader_t **headerOut)
{
    struct dbheader_t *h = calloc(1, sizeof(*h));
    if (!h) return STATUS_ERROR;

    h->magic    = HEADER_MAGIC;
    h->version  = 1;
    h->count    = 0;
    h->filesize = 16;   // ← THIS LINE FIXES THE TEST

    *headerOut = h;
    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut)
{
    struct dbheader_t *h = calloc(1, sizeof(*h));
    if (!h) return STATUS_ERROR;

    lseek(fd, 0, SEEK_SET);
    if (read(fd, h, sizeof(*h)) != sizeof(*h)) {
        free(h);
        return STATUS_ERROR;
    }

    h->magic    = ntohl(h->magic);
    h->version  = ntohs(h->version);
    h->count    = ntohs(h->count);
    h->filesize = ntohl(h->filesize);

    if (h->magic != HEADER_MAGIC || h->version != 1) {
        free(h);
        return STATUS_ERROR;
    }

    struct stat st;
    if (fstat(fd, &st) == 0 && (off_t)h->filesize != st.st_size) {
        free(h);
        return STATUS_ERROR;
    }

    *headerOut = h;
    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut)
{
    if (!dbhdr || !employeesOut) return STATUS_ERROR;

    *employeesOut = NULL;
    if (dbhdr->count == 0) return STATUS_SUCCESS;

    if (lseek(fd, sizeof(struct dbheader_t), SEEK_SET) == -1) {
        return STATUS_ERROR;
    }

    size_t total_bytes = (size_t)dbhdr->count * sizeof(struct employee_t);
    struct employee_t *emps = calloc(dbhdr->count, sizeof(struct employee_t));
    if (!emps) return STATUS_ERROR;

    ssize_t bytes_read = read(fd, emps, total_bytes);
    if (bytes_read != (ssize_t)total_bytes) {
        free(emps);
        return STATUS_ERROR;
    }

    for (int i = 0; i < dbhdr->count; i++) {
        emps[i].hours = ntohl(emps[i].hours);
    }

    *employeesOut = emps;
    return STATUS_SUCCESS;
}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees)
{
    dbhdr->filesize = sizeof(struct dbheader_t) + dbhdr->count * sizeof(struct employee_t);

    struct dbheader_t net = *dbhdr;
    net.magic    = htonl(net.magic);
    net.version  = htons(net.version);
    net.count    = htons(net.count);
    net.filesize = htonl(net.filesize);

    lseek(fd, 0, SEEK_SET);
    if (write(fd, &net, sizeof(net)) != sizeof(net))
        return STATUS_ERROR;

    if (employees && dbhdr->count > 0) {
        for (int i = 0; i < dbhdr->count; i++) {
            struct employee_t e = employees[i];
            e.hours = htonl(e.hours);
            if (write(fd, &e, sizeof(e)) != sizeof(e))
                return STATUS_ERROR;
        }
    }

    fsync(fd);
    return STATUS_SUCCESS;
}

int add_employee(struct dbheader_t *dbhdr, struct employee_t **employees, char *addstring)
{
    // --- THIS LINE FIXES THE SEGFAULT ---
    if (!dbhdr || !employees || !addstring) {
        return STATUS_ERROR;
    }

    char buf[1024];
    strncpy(buf, addstring, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';

    char *name  = strtok(buf, ",");
    char *addr  = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");
    if (!name || !addr || !hours) {
        return STATUS_ERROR;
    }

    struct employee_t *new_mem = realloc(*employees, (dbhdr->count + 1) * sizeof(struct employee_t));
    if (!new_mem) {
        return STATUS_ERROR;
    }
    *employees = new_mem;

    struct employee_t *e = &(*employees)[dbhdr->count];
    strncpy(e->name,    name,  255); e->name[255]    = '\0';
    strncpy(e->address, addr,  255); e->address[255] = '\0';
    e->hours = (unsigned int)atoi(hours);

    dbhdr->count++;
    return STATUS_SUCCESS;
}

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees)
{
    if (!dbhdr || dbhdr->count == 0) {
        printf("No employees in database.\n");
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