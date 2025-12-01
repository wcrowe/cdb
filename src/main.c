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
  printf("\t -a : specify an additional db filename\n");
  printf("\t -f : specify the db filename\n");
}

int main(int argc, char *argv[]) {
  bool newFile = false;
  int c = 0;
  char *filename = NULL;
  int dbfd = -1;
  struct dbheader_t *dbheader = NULL;
  struct employee_t *employees = NULL;

  while ((c = getopt(argc, argv, "anf:")) != -1) {
    switch (c) {
    case 'a':
      /* filename provided in optarg */
    //  (void)optarg;
      filename = optarg;
      break;
    case 'f':
      /* filename provided in optarg */
      filename = optarg;
      //	(void)optarg;
      break;
    case 'n':
      /* -n flag handling */
      newFile = true;
      break;
    default:
      print_usage(argv);
      exit(EXIT_FAILURE);
    }
  }

  if (!newFile && filename == NULL) {
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
  if (output_file(dbfd, dbheader) == STATUS_ERROR) {
    printf("Failed to output file.\n");
    close(dbfd);
    exit(EXIT_FAILURE);
  }
  close(dbfd);
  return STATUS_SUCCESS;
}
