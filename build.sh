#!/bin/sh

gcc -v -Wall -Wextra -Werror -lm -lmpfr -lgmp -I/usr/local/include -L/usr/local/lib src/main.c -o tpi-v2
