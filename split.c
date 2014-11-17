//    Split By Color: a simple tool for a DIY book scanner
//
//    Copyright (C) 2014 InsomniaLab
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "lib/manppm.h"

struct options {
    bool print;
    bool split;
    char *filename;
};

int main(int argc, char *argv[]){
    char c;
    struct options opts;
    opts.filename = NULL;
    opts.print = false;
    opts.split = false;
    while((c = getopt(argc, argv, "f:ps")) != -1) {
        switch(c) {
            case 'f':
                opts.filename = calloc(strlen(optarg) + 1, sizeof(char));
                strncpy(opts.filename, optarg, strlen(optarg) + 1);
                break;
            case 'p':
                opts.print = true;
                break;
            case 's':
                opts.split = true;
                break;
            case '?':
                return 2;
                break;
            default:
                return 2;
                break;
        }
    }
    PPMImage *image;
    line CutLineOut;
    if(!opts.filename || strlen(opts.filename) == 0) {
		
        return 2;
	}
    image = readPPM(opts.filename);
    if(image == NULL) {
        fprintf(stderr, "Error reading PPM\n");
        return 1;
    } else {
		CutLineOut = identifyPPMcutLine(image);
    }
    if(opts.print) {
		printf("Intercept (in pixels): %d\n", CutLineOut.intercept);
		printf("Alpha: %lf\n", CutLineOut.alpha);
    }
    if(opts.split) {
        fillWithBlack(&CutLineOut, image, opts.filename);
    }
    
    free(image->data);
    return 0;
}
