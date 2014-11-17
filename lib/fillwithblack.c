#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fillwithblack.h"

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

void fillWithBlack(line *CutLine, PPMImage *img, const char *filename)
{
    //Init all the needed structs, with two new images
    int i, j;
    PPMImage *newImgLEFT, *newImgRIGHT;
    char *left = "LEFT";
    char *right = "RIGHT";
    char delim[2] = "."; 
    char *name;
    char *ext;
    int new_fn_length = strlen(filename) + strlen(right) + 2;
    char *filenameLEFT = calloc(new_fn_length, sizeof(char));
    char *filenameRIGHT = calloc(new_fn_length, sizeof(char));

    //Prepends the {LEFT,RIGHT} to the filename of the new images
    name = strtok((char *) filename, delim);
    ext = strtok(NULL,delim);

    snprintf(filenameLEFT, new_fn_length, "%s%s.%s",name,left,ext);
    snprintf(filenameRIGHT, new_fn_length, "%s%s.%s",name,right,ext);

    //malloc of the new images
    newImgLEFT = (PPMImage*) malloc(sizeof(PPMImage));
    newImgLEFT->data = (PPMPixel*) malloc(img->x*img->y*sizeof(PPMPixel));
    newImgLEFT->x = img->x;
    newImgLEFT->y = img->y;    

    newImgRIGHT = (PPMImage*) malloc(sizeof(PPMImage));
    newImgRIGHT->data = (PPMPixel*) malloc(img->x*img->y*sizeof(PPMPixel));
    newImgRIGHT->x = img->x;
    newImgRIGHT->y = img->y;    

    //Divides the pixels in LEFT and RIGHT images
    for(j=0;j<img->y;j++)
    {
        for(i=0;i<img->x;i++)
        {
            if(i < j*CutLine->alpha+CutLine->intercept)
            {
                newImgLEFT->data[j*img->x+i] = img->data[j*img->x+i];
                newImgRIGHT->data[j*img->x+i].red = (unsigned char)0;
                newImgRIGHT->data[j*img->x+i].green = (unsigned char)0;
                newImgRIGHT->data[j*img->x+i].blue = (unsigned char)0;
            }
            else
            {
                newImgRIGHT->data[j*img->x+i] = img->data[j*img->x+i];
                newImgLEFT->data[j*img->x+i].red = (unsigned char)0;
                newImgLEFT->data[j*img->x+i].green = (unsigned char)0;
                newImgLEFT->data[j*img->x+i].blue = (unsigned char)0;
            }
        }
    }
    
    //Writes the new images and frees the malloc'd structs
    writePPM(filenameLEFT,newImgLEFT);
    free(filenameLEFT);
    writePPM(filenameRIGHT,newImgRIGHT);
    free(filenameRIGHT);
    free(newImgLEFT->data);
    free(newImgRIGHT->data);
    free(newImgLEFT);
    free(newImgRIGHT);
}
