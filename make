#!/bin/sh
gcc loading.c main.c menu.c about.c intro.c map.c -o superderpy -lallegro -lallegro_audio-debug -lallegro_acodec-debug -lallegro_image-debug -lallegro_font-debug -lallegro_ttf-debug -DDEBUG -std=gnu99 -g -Wall && ./superderpy
