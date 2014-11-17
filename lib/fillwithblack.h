#include "ppm_common.h"

#ifndef _FILL_H
#define _FILL_H

#define CREATOR "INSOMNIALAB"
#define RGB_COMPONENT_COLOR 255

void writePPM(const char *filename, PPMImage *img);
void fillWithBlack(line *CutLine, PPMImage *img, const char *filename);

#endif
