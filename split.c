#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/manipppm.h"

#define CREATOR "RPFELGUEIRAS"
#define RGB_COMPONENT_COLOR 255

typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

typedef struct {
    int intercept;
    double alpha;
} line;

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
		//    changeColorPPM(image);
		//    writePPM("Lucernario2_dots.ppm",image);
		//    printf("Press any key...");
		//    getchar();
		CutLineOut = identifyPPMcutLine(image);
		//    printf("Alpha: %lf", CutLineOut.alpha);
		printf("Intercept (in pixels): %d\n", CutLineOut.intercept);
		printf("Alpha: %lf\n", CutLineOut.alpha);
		//    printf("Alpha: %lf\nIntercept: %d \n",CutLineOut.alpha,CutLineOut.intercept);
    }
    
    
    free(image->data);
}
