#!/bin/sh

#gcc -v -Wall -Wextra -Werror -lm -lmpfr -lgmp -I/usr/include -L/usr/lib src/main.c -o tpi

cmake -S . -B build/ 
cd build/
make 
