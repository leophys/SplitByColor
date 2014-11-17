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
#include "lib/manppm.h"

int main(int argc, char *argv[]){
	char filename[100] = {""};
	int i;
	int count;
    PPMImage *image;
    line CutLineOut;
    if(argc < 2) {
		
		print_help(argv[0]);
		
	} else {
				
		for(i=1;i<=argc-1;i++) {
			if(strncmp(argv[i],"-f",2)==0) {
				count = 1;
			} else if (count > 0 ) {
				if(count == 1) {
					strncpy(filename, argv[i], 100);
				}
				
			} else {
				print_help(argv[0]);
			}
			
		}
	}
    if(strlen(filename) == 0) {
		print_help(argv[0]);
	}
    if(image = readPPM(filename)) {
		CutLineOut = identifyPPMcutLine(image);
		printf("Intercept (in pixels): %d\n", CutLineOut.intercept);
		printf("Alpha: %lf\n", CutLineOut.alpha);
    }
    fillWithBlack(&CutLineOut,image,filename);
    
    free(image->data);
    return 0;
}
