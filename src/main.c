#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "common.h"
#include "file.h"
#include "parse.h"

static void print_usage(const char *progname)
{
    printf("Usage: %s -f <database_file> [options]\n", progname);
    printf("\n");
    printf("Required:\n");
    printf("  -f <file>     Database file path\n");
    printf("\n");
    printf("Options:\n");
    printf("  -n            Create new database (deletes existing)\n");
    printf("  -a \"name,address,hours\"   Add employee\n");
    printf("  -l            List all employees\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -f employees.db -n\n", progname);
    printf("  %s -f employees.db -a \"John Doe,123 Main St,40\" -l\n", progname);
}

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
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    // REQUIRED: -f must be provided
    if (!filename) {
        fprintf(stderr, "Error: -f <database file> is required\n\n");
        print_usage(argv[0]);
        return 1;
    }

    int fd = -1;
    struct dbheader_t *hdr = NULL;
    struct employee_t *emps = NULL;

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

    if (addstring) {
        struct employee_t *tmp = realloc(emps, (hdr->count + 1) * sizeof(*emps));
        if (!tmp) {
            perror("realloc");
            return 1;
        }
        emps = tmp;
        add_employee(hdr, emps, addstring);
    }

    if (do_list)
        list_employees(hdr, emps);

    if (addstring)
        output_file(fd, hdr, emps);

    close(fd);
    free(hdr);
    if (emps) free(emps);

    return 0;
}