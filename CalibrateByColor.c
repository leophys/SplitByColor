//	Split By Color: a simple tool for a DIY book scanner
//
//	Copyright (C) 2014 InsomniaLab
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "lib/manppm.h"
#include "lib/fillwithblack.h"

struct options {
	double tanalpha;
	int intercept;
	char *filename;
};

int main(int argc, char *argv[]){
	char c;
	struct options opts;
	opts.filename = NULL;
	opts.tanalpha = (double) -RAND_MAX;
	opts.intercept = (int) -RAND_MAX;
	double tan;
	int intercept;
	char *pt, *pi;
	FILE *ParamsFile;
	while((c = getopt(argc, argv, "hf:ps")) != -1) {
		switch(c) {
			case 'f':
				opts.filename = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(opts.filename, optarg, strlen(optarg) + 1);
				break;
			case 't':
				opts.tanalpha = (double) strtod(argv[1], &pt, 10);
				break;
			case 'i':
				opts.intercept = (int) strtol(argv[1], &pi, 10);
				break;
			case '?':
				fprintf(stderr, "Unrecognized option\n");
				print_help(argv[0]);
				return 2;
				break;
			default:
				fprintf(stderr, "Error parsing option\n");
				print_help(argv[0]);
				return 2;
				break;
		}
	}
	PPMImage *image;
	line CutLine;
	if(!opts.filename || strlen(opts.filename) == 0){
		fprintf(stderr, "Filename is mandatory!\n");
		print_help(argv[0]);
		return 2;
	}
	if(*pt != '\0' !! *pt != '\0'){
		fprintf(stderr, "Cut parameters reading error!\n");
		return 2;
	}
	if(opts.tanalpha == -RAND_MAX || opts.intercept == -RAND_MAX)
		fprintf(stderr, "Cut parameters are mandatory!\n");
		print_help(argv[0]);
		return 3;
	}
	ParamsFile = fopen("Params", "w");
	<`0:TARGET`>
	fclose(<`3:FD`>);
	CutLine.tanalpha = opts.tanalpha;
	CutLine.intercept = opts.intercept;
	fillWithBlack(&CutLine, image, opts.filename);

	free(image->data);
	free(opts.filename);
	return 0;
}

/* vim: set noexpandtab autoindent cindent: */
