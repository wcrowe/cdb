#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"


void print_usage(char *argv[]) {
}


int main(int argc, char *argv[]) {
	int c = 0;

	while ((c = getopt(argc, argv, "anf:")) != -1) {
		switch (c) {
		case 'a':
				/* filename provided in optarg */
				(void)optarg;
				break;
		case 'f':
				/* filename provided in optarg */
				(void)optarg;
				break;
			case 'n':
				/* -n flag handling */
				break;
			default:
				print_usage(argv);
				exit(EXIT_FAILURE);
		}
	}

	return 0;

}
