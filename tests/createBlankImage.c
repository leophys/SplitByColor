#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../lib/ppm_common.h"
#include "../lib/manppm.h"
#include "../lib/fillwithblack.h"

struct options {
	int width;
	int height;
	bool x1flag;
	bool y1flag;
	bool x2flag;
	bool y2flag;
	int x1;
	int y1;
	int x2;
	int y2;
	char *filename;
};

int main(int argc, char *argv[]){
	int i, j;
	char opt;
	static char *defaultname = "blankTestImg.ppm";
	struct options opts;
	PPMImage *blankImg;
	opts.filename = defaultname;
	opts.width = (int) 600;
	opts.height = (int) 400;
	opts.x1 = -1;
	opts.y1 = -1;
	opts.x2 = -1;
	opts.y2 = -1;
	opts.x1flag = false;
	opts.y1flag = false;
	opts.x2flag = false;
	opts.y2flag = false;
	while((opt = getopt(argc, argv, "n:w:h:x:y:a:b:")) != -1) {
		switch(opt) {
			case 'n':
				opts.filename = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(opts.filename, optarg, strlen(optarg) + 1);
				break;
			case 'w':
				opts.width = (int) atoi(optarg);
				break;
			case 'h':
				opts.height = (int) atoi(optarg);
				break;
			case 'x':
				opts.x1 = (int) atoi(optarg);
				opts.x1flag = true;
				break;
			case 'y':
				opts.y1 = (int) atoi(optarg);
				opts.y1flag = true;
				break;
			case 'a':
				opts.x2 = (int) atoi(optarg);
				opts.x2flag = true;
				break;
			case 'b':
				opts.y2 = (int) atoi(optarg);
				opts.y2flag = true;
				break;
			case '?':
				fprintf(stderr, "Unrecognized option\n");
				return 2;
				break; 
			default:
				printf("Used default values for test:\n");
				printf("width=%d\t height=%d\n",opts.width,opts.height);
				printf("filename: %s\n",opts.filename);
				break;
		}
	}
//	printf("w: %d\t h: %d\n",opts.width,opts.height);
//	printf("x1: %d\t y1: %d\n",opts.x1,opts.y1);
//	printf("x2: %d\t y2: %d\n",opts.x2,opts.y2);
//	printf("filename: %s\n",opts.filename);
	if(opts.width>0 && opts.height>0){
		blankImg = (PPMImage*) malloc(sizeof(PPMImage));
		blankImg->x = opts.width;
		blankImg->y = opts.height;
		blankImg->data = (PPMPixel*) malloc(opts.width*opts.height*sizeof(PPMPixel));
		for(i=0;i<opts.width;i++){
			for(j=0;j<opts.height;j++){
				blankImg->data[j*opts.width+i].red = (char) RGB_COMPONENT_COLOR;
				blankImg->data[j*opts.width+i].green = (char) RGB_COMPONENT_COLOR;
				blankImg->data[j*opts.width+i].blue = (char) RGB_COMPONENT_COLOR;
			}
		}
	}
	else{
		printf("Error! Width and/or height are not positive integer numbers!\n");
		exit(EXIT_FAILURE);
	}
	if(opts.x1 != -1 && opts.y1 != -1 && opts.x2 != -1 && opts.y2 != -1){
		if(opts.x1flag && opts.y1flag && opts.x2flag && opts.y2flag){
			if(opts.x1 < opts.width && opts.y1 < opts.height
					&& opts.x2 < opts.width && opts.y2 < opts.height){
				blankImg->data[opts.y1*opts.width+opts.x1].red = (char) 0;
				blankImg->data[opts.y1*opts.width+opts.x1].green = (char) RGB_COMPONENT_COLOR;
				blankImg->data[opts.y1*opts.width+opts.x1].blue = (char) 0;

				blankImg->data[opts.y2*opts.width+opts.x2].red = (char) 0;
				blankImg->data[opts.y2*opts.width+opts.x2].green = (char) RGB_COMPONENT_COLOR;
				blankImg->data[opts.y2*opts.width+opts.x2].blue = (char) 0;
			}
			else{
				printf("Error! Green dots out of picture range!\n");
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Error! Not all the dots' coordinates inserted!\n");
			exit(EXIT_FAILURE);
		}
	}

	writePPM(opts.filename,blankImg);
	free(blankImg->data);
	free(blankImg);
	return 0;
}
