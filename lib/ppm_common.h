#ifndef _PPM_H
#define _PPM_H
#ifndef greenT
#define greenT 100
#endif
#ifndef redT
#define redT 100
#endif
#ifndef blueT
#define blueT 100
#endif

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

#endif
