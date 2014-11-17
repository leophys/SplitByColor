#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "manppm.h"
#define RGB_COMPONENT_COLOR 255

void print_help(char *name) {
	printf("Usage: %s -f image.ppm\n", name);
	exit(EXIT_FAILURE);
}

PPMImage *readPPM(const char *filename) {
	char buff[16];
	PPMImage *img;
	FILE *fp;
	int c, rgb_comp_color;
	//open PPM file for reading
	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}
	
	//read image format
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		exit(1);
	}

	//check the image format
	if (buff[0] != 'P' || buff[1] != '6') {
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp);
    return img;
}

bool threshold(int green, int red, int blue) // Outputs true if the
{                                            // pixel is above the threshold
    static int greenT=180, redT=0, blueT=0;

    // Test the pixel with the threshold
    if(green >= 0 && green <= 255 && red >= 0 && red <= 255 && blue >= 0 && blue <= 255) {
        
        if(green>=greenT && red<=redT && blue<=blueT) {
            return 1;
            
        } else {
            return 0;
            
        }
    } else {
        
        printf("Error: Ureadable pixel colors");
        exit(2);
        
    }
}

line identifyPPMcutLine(PPMImage *img) {// Outputs the cut line parameters
										// alpha and intercept
    line CutLine;
    int *pixelPositionX, *pixelPositionY;
    int *tempX, *tempY;
    int xCenterUP, yCenterUP, xCenterDOWN, yCenterDOWN;
    int xMiddle, i, j, k, green, red, blue;
    int kUP, kDOWN;
    pixelPositionX = (int *)calloc(1, sizeof(int));
    pixelPositionY = (int *)calloc(1, sizeof(int));
    k=0;

    //Identifies the coordinates of the above-threshold points
	for(j=0;j<img->y;j++){
		for(i=0;i<img->x;i++) {		
		
			if(threshold((int)img->data[j*img->x + i].green, (int)img->data[j*img->x + i].red, (int)img->data[j*img->x + i].blue)) {
				tempX = (int *) realloc(pixelPositionX,(k+1)*sizeof(int));
                tempY = (int *) realloc(pixelPositionY,(k+1)*sizeof(int));
                if(tempX || tempY) {
					pixelPositionX = tempX;
                    pixelPositionY = tempY;
                    pixelPositionX[k] = j;
                    pixelPositionY[k] = i;
                    k++;
				} else {
                    printf("Error: Failed realloc of memory");
                    exit(3);
                }
			}
		}		
	}
	
	if(k == 0) {
		printf("No green points are detected!\n");
		exit(EXIT_FAILURE);
	}
	
    //Geometric needs to distinguish the two points
    //and find the barycenter of the two separately
    xMiddle = (int) (img->y)/2.;
    xCenterUP = 0;
    yCenterUP = 0;
    xCenterDOWN = 0;
    yCenterDOWN = 0;
    kUP = kDOWN = 0;

    //Calculates the barycenters of the two points
    for(i=0;i<k;i++) {
        if(pixelPositionX[i] > xMiddle) {
			kUP++;
			xCenterUP+=pixelPositionX[i];
            yCenterUP+=pixelPositionY[i];
            
        } else {
			kDOWN++;
            xCenterDOWN+=pixelPositionX[i];
            yCenterDOWN+=pixelPositionY[i];
            
        }
    }
    xCenterUP = (int) xCenterUP/kUP;
    yCenterUP = (int) yCenterUP/kUP;
    xCenterDOWN = (int) xCenterDOWN/kDOWN;
    yCenterDOWN = (int) yCenterDOWN/kDOWN;
	
	/*printf("xmiddle: %d\n", xMiddle);
	printf("xcenterUP: %d\tycenterUP: %d\n", xCenterUP, yCenterUP);
	printf("xcenterDOWN: %d\tycenterDOWN: %d\n", xCenterDOWN, yCenterDOWN);*/
	

    //Assigns the Intercept and the angle
    CutLine.alpha = (double) atan((double) (yCenterDOWN - yCenterUP)/(xCenterDOWN - xCenterUP));
    CutLine.intercept = (int) (yCenterUP - CutLine.alpha * xCenterUP);
    
    return(CutLine);
    
    free(pixelPositionX);
    free(pixelPositionY);
}
