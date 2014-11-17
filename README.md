Split By Color
==============

A simple C program to cut ppm images along a line identified
by two green dots. It is a utility to be integrated in the
InsomniaLab DIY bookscanner.


To compile
----------

gcc split.c lib/manppm.c lib/fillwithblack.c -o split -lm

How to use
----------

./split -f <filename>.ppm

The code is fairly commented.
The code to read and write the PPM is taken from an
example in stackoverflow (http://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c).



TO DO:
------
- Integrate with ppmrose (https://aur.archlinux.org/packages/ppmrose) to perform the unwarping
- Test with real book scanner images
- A LOT of debug!
