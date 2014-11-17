#include "ppm_common.h"

#ifndef _MANPPM_H
#define _MANPPM_H

void print_help(char *name);

PPMImage *readPPM(const char *filename);

bool threshold(int green, int red, int blue);

line identifyPPMcutLine(PPMImage *img);

#endif
