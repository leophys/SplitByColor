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
    
    
    free(image->data);
    return 0;
}
