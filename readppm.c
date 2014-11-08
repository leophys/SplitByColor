#include<stdio.h>
#include<stdlib.h>

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
//    c = getc(fp);
//    while (c == '#') {
//    while (getc(fp) != '\n') ;
//         c = getc(fp);
//    }

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
 threshold(PPMPixel* imgPixel) // Outputs true if the
{                                  // pixel is above the threshold
    int green, red, blue;
    static int greenT=180, redT=20, blueT=20;

    // Convert the hex char value in int
    green = (int)strtol(imgPixel->green, NULL, 16);
    blue = (int)strtol(imgPixel->blue, NULL, 16);
    red = (int)strtol(imgPixel->red, NULL, 16);
    blue = (int)strtol(imgPixel->blue, NULL, 16);
    red = (int)strtol(imgPixel->red, NULL, 16);
    blue = (int)strtol(imgPixel->blue, NULL, 16);

    // Test the pixel with the threshold
    if(green = imgPixel->green &&
            red = imgPixel->red && 
            blue = imgPixel->blue)
    {
        if(green>greenT && red<redT && blue<blueT)
        {
            return 1;
        }
        else
        {
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
    PPMPixel* imgPixel;
    line CutLine;
    int* pixelPosition, xCenterUP, yCenterUP, xCenterDOWN, yCenterDOWN;
    int xMiddle, i, j, k;

    imgPixel = &(img->data);
    k=0;

    for(i=0;i<img->x;i++)
    {
        for(j=0;j<img->y;j++)
        {
            if(threshold(imgPixel[i][j]))
            {
                realloc(pixelPosition,sizeof(int));
                pixelPosition[1][k] = i;
                pixelPosition[2][k] = j;
                k++;
            }
        }
    }

    xMiddle = (int) (img->y)/2.;
    xCenterUP = 0;
    yCenterUP = 0;
    xCenterDOWN = 0;
    yCenterDOWN = 0;


    for(i=0;i<k;i++)
    {
        if(pixelPosition[1][i]>xMiddle)
        {
            xCenterUP+=pixelPosition[1][i];
            yCenterUP+=pixelPosition[2][i];
        }
        else
        {
            xCenterDOWN+=pixelPosition[1][i];
            yCenterDOWN+=pixelPosition[2][i];
        }
    }
    xCenterUP = (int) xCenterUP/k;
    yCenterUP = (int) yCenterUP/k;
    xCenterDOWN = (int) xCenterDOWN/k;
    yCenterDOWN = (int) yCenterDOWN/k;

    CutLine->alpha = (double) arctan()
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
    image = readPPM("can_bottom.ppm");
    changeColorPPM(image);
    writePPM("can_bottom2.ppm",image);
    printf("Press any key...");
    getchar();
}
