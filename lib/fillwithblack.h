#ifndef _FILL_H
#define _FILL_H

#define CREATOR "INSOMNIALAB"
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


void writePPM(const char *filename, PPMImage *img);
void fillWithBlack(line *CutLine, PPMImage *img, const char *filename);

#endif
