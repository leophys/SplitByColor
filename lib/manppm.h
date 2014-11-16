#ifndef _MANPPM_H
#define _MANPPM_H

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

void print_help(char *name);

PPMImage *readPPM(const char *filename);

bool threshold(int green, int red, int blue);

line identifyPPMcutLine(PPMImage *img);

#endif
