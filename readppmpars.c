#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

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

void print_help(char *name) {
	printf("Usage: %s -f image.ppm\n", name);
	exit(EXIT_FAILURE);
}

static PPMImage *readPPM(const char *filename) {
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
            
            //printf("Perso\n");
            return 0;
            
        }
    } else {
        
        printf("Error: Ureadable pixel colors");
        exit(2);
        
    }
}

line identifyPPMcutLine(PPMImage *img) { 	// Outputs the cut line parameters
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
	
    xMiddle = (int) (img->y)/2.;
    xCenterUP = 0;
    yCenterUP = 0;
    xCenterDOWN = 0;
    yCenterDOWN = 0;
    kUP = kDOWN = 0;

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
	
	printf("xmiddle: %d\n", xMiddle);
	printf("xcenterUP: %d\tycenterUP: %d\n", xCenterUP, yCenterUP);
	printf("xcenterDOWN: %d\tycenterDOWN: %d\n", xCenterDOWN, yCenterDOWN);
	
    CutLine.alpha = (double) atan((double) (yCenterDOWN - yCenterUP)/(xCenterDOWN - xCenterUP));
    CutLine.intercept = (int) (yCenterUP - CutLine.alpha * xCenterUP);
    
    return(CutLine);
    
    free(pixelPositionX);
    free(pixelPositionY);
}

void writePPM(const char *filename, PPMImage *img) {
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //comments
    fprintf(fp, "# Created by %s\n",CREATOR);

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}

void changeColorPPM(PPMImage *img)
{
    int i;
    if(img){

         for(i=0;i<img->x*img->y;i++){
              img->data[i].red=RGB_COMPONENT_COLOR-img->data[i].red;
              img->data[i].green=RGB_COMPONENT_COLOR-img->data[i].green;
              img->data[i].blue=RGB_COMPONENT_COLOR-img->data[i].blue;
         }
    }
}

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
