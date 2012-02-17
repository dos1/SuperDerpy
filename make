#!/bin/sh
gcc loading.c main.c menu.c about.c intro.c -o superderpy -lallegro -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_font -lallegro_ttf -std=gnu99 -g && ./superderpy
