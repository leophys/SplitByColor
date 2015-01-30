Split By Color
==============

A simple C program to cut ppm images along a line identified
by two green dots. It is a utility to be integrated in the
InsomniaLab DIY bookscanner.


To compile
----------

Simply use make.
To compile

make all

To clean

make clean

How to use
----------

./split -f <filename>.ppm

The code is fairly commented.
The code to read and write the PPM is taken from an
example in stackoverflow (http://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c).

Debug
-----

- threshold(int green, int red, int blue) [lib/manppm.c]

This is the function that tests the color of any pixel that is passed.
The logic is the simplest: to pass the test the color should be into
the interval

green<=greenT
red>=redT
blue<=blueT

The three constants (greenT,redT,blueT) are defined in lib/ppm_common.h

To stress or test the threshold function in lib/manppm.c you can
change the definition at compile time with

make EXTRACFLAGS="-D greenT=new_green_value -D redT=new_red_value
-D blueT=new_blue_value" all



TO DO:
------
- Integrate with ppmrose (https://aur.archlinux.org/packages/ppmrose) to perform the unwarping
- Test with real book scanner images (in progress)
- A LOT of debug!
