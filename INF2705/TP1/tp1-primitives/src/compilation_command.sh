#!/bin/bash

g++ main.cpp -I/usr/include/GL -I/usr/include/libdrm  -I/usr/include/SDL2 -D_REENTRANT -g -W -Wall -Wno-unused-parameter -Wno-deprecated-declarations -lGLEW -lGLU -lGL  -L/usr/lib/x86_64-linux-gnu -lSDL2 -lGLU -lGL -o main.exe
