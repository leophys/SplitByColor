Split By Color
==============

To compile

gcc -o out.o readppm.c -lm

Code is fairly commented.
The code to read the PPM is taken from an
example in stackoverflow.
The original functions are:
"bool threshold(int,int,int);"
"line identifyPPMcutLine(PPMImage*);"



TO DO:
There is a problem in execution.
The run never runs into the cycle which
adds the green points.

