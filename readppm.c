#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

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


#define CREATOR "RPFELGUEIRAS"
#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename)
{
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
    static int greenT=180, redT=20, blueT=20;

    // Test the pixel with the threshold
    if(green >= 0 && green <= 255 &&
                    red >= 0 && red <= 255 &&
                    blue >= 0 && blue <= 255)
    {
        if(green>greenT && red<redT && blue<blueT)
        {
            printf("Vinto\n");
            return 1;
        }
        else
        {
            //printf("Perso\n");
            return 0;
        }
    }
    else
    {
        printf("Error: Ureadable pixel colors");
        exit(2);
    }
}

line identifyPPMcutLine(PPMImage *img) // Outputs the cut line parameters
{                                      // alpha and intercept
    line CutLine;
    int *pixelPositionX, *pixelPositionY;
    int *tempX, *tempY;
    int xCenterUP, yCenterUP, xCenterDOWN, yCenterDOWN;
    int xMiddle, i, j, k, green, red, blue;
    pixelPositionX = (int *)calloc(1, sizeof(int));
    pixelPositionY = (int *)calloc(1, sizeof(int));
    
    printf("Qui ci arrivo 1.a\n");

    printf("MannagioCristo: %d\n",(int)img->data[385*148].green);

    k=0;

    for(i=0;i<img->x;i++)
    {
        for(j=0;j<img->y;j++)
        {
					//printf("k: %d\n", k);
			//printf("red: %d\tgreen: %d\tblue: %d\t coord: %d\t%d\n", img->data[(i+1)*(j+1)].red, img->data[(i+1)*(j+1)].green, img->data[(i+1)*(j+1)].blue, i, j);
                if(threshold((int)img->data[(i+1)*(j+1)].green,
                        (int)img->data[(i+1)*(j+1)].red,
                        (int)img->data[(i+1)*(j+1)].blue))
                {
                        printf("PorcoDio\n");
                        tempX = (int *) realloc(pixelPositionX,(k+1)*sizeof(int));
                        tempY = (int *) realloc(pixelPositionY,(k+1)*sizeof(int));
                        //if(!tempX || !tempY)
                        //{
                                pixelPositionX = tempX;
                                pixelPositionY = tempY;
                                pixelPositionX[k] = i;
                                pixelPositionY[k] = j;
                                k++;
                                printf("k = %d\n",k);
                        //}
                        /*else
                        {
                                printf("Error: Failed realloc of memory");
                                exit(3);
                        }*/
                }
        }
    }


    xMiddle = (int) (img->y)/2.;
    xCenterUP = 0;
    yCenterUP = 0;
    xCenterDOWN = 0;
    yCenterDOWN = 0;


    printf("Qui ci arrivo 1.b\n");
    printf("k: %d",k);
    for(i=0;i<k;i++)
    {
        printf("Qui ci arrivo: ciclo %d\n",i);
        if(pixelPositionX[i]>xMiddle)
        {
            xCenterUP+=pixelPositionX[i];
            yCenterUP+=pixelPositionY[i];
        }
        else
        {
            xCenterDOWN+=pixelPositionX[i];
            yCenterDOWN+=pixelPositionY[i];
        }
    }
    xCenterUP = (int) xCenterUP/k;
    yCenterUP = (int) yCenterUP/k;
    xCenterDOWN = (int) xCenterDOWN/k;
    yCenterDOWN = (int) yCenterDOWN/k;


    CutLine.alpha = (double) atan((double) (yCenterUP - yCenterDOWN)/(xCenterUP - xCenterDOWN));
    CutLine.intercept = (int) (yCenterUP - CutLine.alpha * xCenterUP);
    
    return(CutLine);
    
    free(pixelPositionX);
    free(pixelPositionY);
}

void writePPM(const char *filename, PPMImage *img)
{
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

int main(){
    PPMImage *image;
    line CutLineOut;
    if(image = readPPM("Lucernario_dots.ppm"))
    {
            //    changeColorPPM(image);
            //    writePPM("Lucernario2_dots.ppm",image);
            //    printf("Press any key...");
            //    getchar();
            printf("Qui ci arrivo 1\n");
            CutLineOut = identifyPPMcutLine(image);
            printf("Qui ci arrivo 2\n");
            //    printf("Alpha: %lf", CutLineOut.alpha);
            printf("Intercept (in pixels): %d\n", CutLineOut.intercept);
            printf("Alpha: %lf\n", CutLineOut.alpha);
            //    printf("Alpha: %lf\nIntercept: %d \n",CutLineOut.alpha,CutLineOut.intercept);
    }
    
    
    free(image->data);
}
