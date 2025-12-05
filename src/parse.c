#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "common.h"
#include "parse.h"

int create_db_header(struct dbheader_t **out_header)
{
    struct dbheader_t *h = calloc(1, sizeof(*h));
    if (!h) return STATUS_ERROR;

    h->magic    = HEADER_MAGIC;
    h->version  = 1;
    h->count    = 0;
    h->filesize = 16;  /* Fixed header size */

    *out_header = h;
    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **out_header)
{
    *out_header = NULL;
    struct dbheader_t *h = calloc(1, sizeof(*h));
    if (!h) return STATUS_ERROR;

    if (read(fd, h, 16) != 16) {
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
    if (fstat(fd, &st) == 0 && h->filesize != (unsigned int)st.st_size) {
        free(h);
        return STATUS_ERROR;
    }

    *out_header = h;
    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *hdr, struct employee_t **out_employees)
{
    if (hdr->count == 0) {
        *out_employees = NULL;
        return STATUS_SUCCESS;
    }

    lseek(fd, 16, SEEK_SET);
    struct employee_t *emps = calloc(hdr->count, sizeof(*emps));
    if (!emps) return STATUS_ERROR;

    read(fd, emps, hdr->count * sizeof(*emps));

    for (int i = 0; i < hdr->count; i++)
        emps[i].hours = ntohl(emps[i].hours);

    *out_employees = emps;
    return STATUS_SUCCESS;
}

int output_file(int fd, struct dbheader_t *hdr, struct employee_t *employees)
{
    hdr->filesize = 16 + hdr->count * sizeof(struct employee_t);

    struct dbheader_t net = *hdr;
    net.magic    = htonl(net.magic);
    net.version  = htons(net.version);
    net.count    = htons(net.count);
    net.filesize = htonl(net.filesize);

    lseek(fd, 0, SEEK_SET);
    write(fd, &net, 16);

    for (int i = 0; i < hdr->count; i++) {
        struct employee_t e = employees[i];
        e.hours = htonl(e.hours);
        write(fd, &e, sizeof(e));
    }

    fsync(fd);
    return STATUS_SUCCESS;
}

int add_employee(struct dbheader_t *hdr, struct employee_t *employees, char *addstring)
{
    char *name  = strtok(addstring, ",");
    char *addr  = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");

    if (!name || !addr || !hours) return STATUS_ERROR;

    strncpy(employees[hdr->count].name,    name,  255); employees[hdr->count].name[255]    = '\0';
    strncpy(employees[hdr->count].address, addr,  255); employees[hdr->count].address[255] = '\0';
    employees[hdr->count].hours = atoi(hours);
    hdr->count++;

    return STATUS_SUCCESS;
}

void list_employees(struct dbheader_t *hdr, struct employee_t *employees)
{
    for (int i = 0; i < hdr->count; i++) {
        printf("Employee %d\n", i);
        printf("\tName: %s\n", employees[i].name);
        printf("\tAddress: %s\n", employees[i].address);
        printf("\tHours: %d\n", employees[i].hours);
    }
}