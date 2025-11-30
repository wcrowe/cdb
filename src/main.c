#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

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

	while ((c = getopt(argc, argv, "anf:")) != -1) {
		switch (c) {
		case 'a':
				/* filename provided in optarg */
				(void)optarg;
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
        printf("Creating a new file...%d\n", newFile);
    } else {
        dbfd = open_db_file(filename);
        if (dbfd == STATUS_ERROR) {
            exit(EXIT_FAILURE);
        }
        printf("Opening existing file...%d\n",filename != NULL );
    }

	return STATUS_SUCCESS;

}
