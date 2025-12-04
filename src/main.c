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
    printf("Usage: %s -f <dbfile> [-n] [-a \"name,address,hours\"] [-l]\n", progname);
    printf("  -f <file>   Database file (required)\n");
    printf("  -n          Create new database\n");
    printf("  -a <str>    Add employee\n");
    printf("  -l          List all employees\n");
}

int main(int argc, char *argv[])
{
    int opt;
    bool create_new = false;
    bool do_list    = false;
    char *filename  = NULL;
    char *addstring = NULL;

    while ((opt = getopt(argc, argv, "f:na:l")) != -1) {
        switch (opt) {
            case 'f': filename   = optarg; break;
            case 'n': create_new = true;   break;
            case 'a': addstring  = optarg; break;
            case 'l': do_list    = true;   break;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!filename) {
        fprintf(stderr, "Error: -f <database file> is required.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int fd = -1;
    struct dbheader_t  *dbhdr = NULL;
    struct employee_t  *employees = NULL;

    if (create_new) {
        fd = create_db_file(filename);
        if (fd == STATUS_ERROR) return EXIT_FAILURE;

        if (create_db_header(&dbhdr) == STATUS_ERROR) {
            close(fd);
            return EXIT_FAILURE;
        }

        // Write exactly 16 bytes of header
        lseek(fd, 0, SEEK_SET);
        if (write(fd, dbhdr, 16) != 16) {           // ← HARDCODED 16
            perror("write initial header");
            close(fd);
            free(dbhdr);
            return EXIT_FAILURE;
        }
    } else {
        fd = open_db_file(filename, &dbhdr);
        if (fd == STATUS_ERROR) return EXIT_FAILURE;
    }

    if (read_employees(fd, dbhdr, &employees) == STATUS_ERROR) {
        fprintf(stderr, "Failed to read employees\n");
        close(fd); free(dbhdr); free(employees);
        return EXIT_FAILURE;
    }

    if (addstring) {
        if (add_employee(dbhdr, &employees, addstring) == STATUS_ERROR) {
            fprintf(stderr, "Failed to add employee\n");
        }
    }

    if (do_list) {
        list_employees(dbhdr, employees);
    }

    // Only save if we added an employee
    if (addstring) {
        if (output_file(fd, dbhdr, employees) == STATUS_ERROR) {
            fprintf(stderr, "Failed to save database\n");
            close(fd); free(dbhdr); free(employees);
            return EXIT_FAILURE;
        }
    }

    close(fd);
    free(dbhdr);
    free(employees);

    return EXIT_SUCCESS;
}