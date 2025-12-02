#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s [-a filename] [-f filename] [-n]\n", argv[0]);
  printf("\t -n : create a new db file\n");
  printf("\t -f : specify the db filename\n");
}

int main(int argc, char *argv[]) {
  bool newFile = false;
  bool list = false;
  int c = 0;
  char *filename = NULL;
  char *addstring = NULL;
  int dbfd = -1;
  struct dbheader_t *dbheader = NULL;
  struct employee_t *employees = NULL;

  while ((c = getopt(argc, argv, "nf:a:l")) != -1) {
    switch (c) {
    case 'n':
      /* -n flag handling */
      newFile = true;
      break;
    case 'f':
      /* filename provided in optarg */
      filename = optarg;
      //	(void)optarg;
      break;
    case 'a':
      /* -n flag handling */
      addstring = optarg;
      break;

    case 'p':
      /* -n flag handling */
      newFile = true;
      break;
    case 'l':
      list = true;
      break;
    case '?':
      printf("Unknown option -%c\n", c);
      break;
    default:
      print_usage(argv);
      exit(EXIT_FAILURE);
    }
  }

  if (filename == NULL) {
    print_usage(argv);
    exit(EXIT_SUCCESS);
  }

  if (newFile) {
    dbfd = create_db_file(filename);
    if (dbfd == STATUS_ERROR) {
      exit(EXIT_FAILURE);
    }
    if (create_db_header(dbfd, &dbheader) == STATUS_ERROR) {
      close(dbfd);
      printf("Failed to create db header.\n");
      exit(EXIT_FAILURE);
    }

  } else {
    dbfd = open_db_file(filename);
    if (dbfd == STATUS_ERROR) {
      printf("Failed to open db file.\n");
      exit(EXIT_FAILURE);
    }
    if (validate_db_header(dbfd, &dbheader) == STATUS_ERROR) {
      close(dbfd);
      printf("Failed to validate db header.\n");
      exit(EXIT_FAILURE);
    }
    printf("Opening existing file...%d\n", filename != NULL);
  }
  if (read_employees(dbfd, dbheader, &employees) == STATUS_ERROR) {
    printf("Failed to read employees from db file.\n");
    close(dbfd);
    exit(EXIT_FAILURE);
  }
  if (addstring != NULL) {
    dbheader->count += 1;
    dbheader->filesize += sizeof(struct employee_t);
    struct employee_t *temp = realloc(employees, dbheader->count * sizeof(struct employee_t));
    if (temp == NULL) {
      printf("Failed to realloc employee array.\n");
      close(dbfd);
      exit(EXIT_FAILURE);
    }
    employees = temp;

    if (add_employee(dbheader, employees, addstring) == STATUS_ERROR) {
      printf("Failed to add employee.\n");
      close(dbfd);
      exit(EXIT_FAILURE);
    }
  }
  if (output_file(dbfd, dbheader, employees) == STATUS_ERROR) {
    printf("Failed to output file.\n");
    close(dbfd);
    exit(EXIT_FAILURE);
  }
  close(dbfd);
  return STATUS_SUCCESS;
}
