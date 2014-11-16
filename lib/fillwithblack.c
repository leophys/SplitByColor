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
    int i, j;
    PPMImage *newImgLEFT, *newImgRIGHT;
    char *left = "LEFT";
    char *right = "RIGHT";
    char filenameLEFT[100];
    char filenameRIGHT[100];


    strncpy(filenameLEFT,filename,100);
    strncpy(filenameRIGHT,filename,100);
    snprintf(filenameLEFT,sizeof(filenameLEFT),"%s%s",left,filename);
    snprintf(filenameRIGHT,sizeof(filenameRIGHT),"%s%s",right,filename);


    newImgLEFT = (PPMImage*) malloc(sizeof(PPMImage));
    newImgLEFT->data = (PPMPixel*) malloc(img->x*img->y*sizeof(PPMPixel));
    newImgLEFT->x = img->x;
    newImgLEFT->y = img->y;    

    newImgRIGHT = (PPMImage*) malloc(sizeof(PPMImage));
    newImgRIGHT->data = (PPMPixel*) malloc(img->x*img->y*sizeof(PPMPixel));
    newImgRIGHT->x = img->x;
    newImgRIGHT->y = img->y;    
 
    for(j=0;j<img->y;j++)
    {
        for(i=0;i<img->x;i++)
        {
            if(i < j*CutLine->alpha+CutLine->intercept)
            {
                newImgLEFT->data[j*img->x+i] = img->data[j*img->x+i];
                newImgRIGHT->data[j*img->x+i].red = (unsigned char)255;
                newImgRIGHT->data[j*img->x+i].green = (unsigned char)255;
                newImgRIGHT->data[j*img->x+i].blue = (unsigned char)255;
            }
            else
            {
                newImgRIGHT->data[j*img->x+i] = img->data[j*img->x+i];
                newImgLEFT->data[j*img->x+i].red = (unsigned char)255;
                newImgLEFT->data[j*img->x+i].green = (unsigned char)255;
                newImgLEFT->data[j*img->x+i].blue = (unsigned char)255;
            }
        }
    }
    
    writePPM(filenameLEFT,newImgLEFT);
    writePPM(filenameRIGHT,newImgRIGHT);
    free(newImgLEFT->data);
    free(newImgRIGHT->data);
    free(newImgLEFT);
    free(newImgRIGHT);
}
