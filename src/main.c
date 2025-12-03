#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"
#include "file.h"
#include "parse.h"

static void print_usage(char *argv[]) {
    printf("Usage: %s -f <database_file> [-n] [-a \"name,address,hours\"] [-l]\n", argv[0]);
    printf("  -n            Create a new database file (if it doesn't exist)\n");
    printf("  -f <file>     Specify database file (required)\n");
    printf("  -a <record>   Add employee: \"Name,Address,Hours\"\n");
    printf("  -l            List all employees\n");
}

int main(int argc, char *argv[]) {
    bool create_new = false;
    bool list_flag  = false;
    char *filename  = NULL;
    char *addstring = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "nf:a:l")) != -1) {
        switch (opt) {
            case 'n': create_new = true;             break;
            case 'f': filename   = optarg;           break;
            case 'a': addstring  = optarg;           break;
            case 'l': list_flag  = true;             break;
            case '?':
                print_usage(argv);
                return EXIT_FAILURE;
            default:
                print_usage(argv);
                return EXIT_FAILURE;
        }
    }

    if (filename == NULL) {
        printf("Error: -f <database_file> is required.\n");
        print_usage(argv);
        return EXIT_FAILURE;
    }

    int dbfd = -1;
    struct dbheader_t *dbheader = NULL;
    struct employee_t *employees = NULL;

    if (create_new) {
        // Create new file + header
        dbfd = create_db_file(filename);
        if (dbfd == STATUS_ERROR) {
            return EXIT_FAILURE;
        }

        if (create_db_header(&dbheader) == STATUS_ERROR) {
            close(dbfd);
            return EXIT_FAILURE;
        }

        // Write initial header to disk
        lseek(dbfd, 0, SEEK_SET);
        struct dbheader_t tmp = *dbheader;
        tmp.magic     = htonl(tmp.magic);
        tmp.version   = htons(tmp.version);
        tmp.count     = htons(tmp.count);
        tmp.filesize  = htonl(tmp.filesize);
        if (write(dbfd, &tmp, sizeof(tmp)) != sizeof(tmp)) {
            perror("write header");
            close(dbfd);
            free(dbheader);
            return EXIT_FAILURE;
        }

    } else {
        // Open existing file
        dbfd = open_db_file(filename, &dbheader);
        if (dbfd == STATUS_ERROR) {
            return EXIT_FAILURE;
        }
    }

    // At this point:
    // - dbfd is valid
    // - dbheader is fully parsed and in host byte order
    // - file pointer is right after the header (ready for read_employees)

    if (read_employees(dbfd, dbheader, &employees) == STATUS_ERROR) {
        printf("Failed to read employees.\n");
        close(dbfd);
        free(dbheader);
        free(employees);
        return EXIT_FAILURE;
    }

    // Add new employee if requested
    if (addstring != NULL) {
        dbheader->count += 1;

        // Realloc employee array
        struct employee_t *temp = realloc(employees,
            dbheader->count * sizeof(struct employee_t));
        if (temp == NULL) {
            perror("realloc");
            dbheader->count -= 1;  // rollback
        } else {
            employees = temp;
            if (add_employee(dbheader, employees, addstring) == STATUS_ERROR) {
                printf("Failed to add employee.\n");
                dbheader->count -= 1;  // rollback count
            }
        }
    }

    // List employees if requested
    if (list_flag) {
        list_employees(dbheader, employees);
    }

    // Write everything back to disk
    if (output_file(dbfd, dbheader, employees) == STATUS_ERROR) {
        printf("Failed to write database.\n");
        close(dbfd);
        free(dbheader);
        free(employees);
        return EXIT_FAILURE;
    }

    // Cleanup
    close(dbfd);
    free(dbheader);
    free(employees);

    return EXIT_SUCCESS;
}