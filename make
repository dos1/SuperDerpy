#!/bin/sh
gcc main.c menu.c loading.c -o superderpy -lallegro -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_font -lallegro_ttf -std=gnu99 -g && ./superderpy
