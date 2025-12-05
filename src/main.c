#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "common.h"
#include "file.h"
#include "parse.h"

int main(int argc, char *argv[])
{
    const char *filename = NULL;
    bool create_new = false;
    bool do_list = false;
    char *addstring = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "f:na:l")) != -1) {
        switch (opt) {
            case 'f': filename   = optarg; break;
            case 'n': create_new = true;   break;
            case 'a': addstring  = optarg; break;
            case 'l': do_list    = true;   break;
            default: return 1;
        }
    }

    if (!filename) return 1;

    int fd = -1;
    struct dbheader_t *hdr = NULL;
    struct employee_t *emps = NULL;

    /* Create or open database */
    if (create_new) {
        fd = create_db_file(filename);
        if (fd == STATUS_ERROR) return 1;

        create_db_header(&hdr);

        struct dbheader_t net = *hdr;
        net.magic    = htonl(net.magic);
        net.version  = htons(net.version);
        net.count    = htons(net.count);
        net.filesize = htonl(net.filesize);

        lseek(fd, 0, SEEK_SET);
        write(fd, &net, 16);
    } else {
        fd = open_db_file(filename);
        if (fd == STATUS_ERROR) return 1;

        validate_db_header(fd, &hdr);
    }

    read_employees(fd, hdr, &emps);

    /* Add employee — DO NOT increment count here */
    if (addstring) {
        /* Grow array first */
        struct employee_t *tmp = realloc(emps, (hdr->count + 1) * sizeof(*emps));
        if (!tmp) {
            perror("realloc");
            return 1;
        }
        emps = tmp;

        /* Let add_employee use hdr->count as index and increment it */
        add_employee(hdr, emps, addstring);
    }

    if (do_list)
        list_employees(hdr, emps);

    if (addstring)
        output_file(fd, hdr, emps);

    close(fd);
    free(hdr);
    if (emps) free(emps);  /* Safe free */

    return 0;
}