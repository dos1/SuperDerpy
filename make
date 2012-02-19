#!/bin/sh
gcc src/loading.c src/main.c src/menu.c src/about.c src/intro.c src/map.c -o bin/superderpy -lallegro -lallegro_audio-debug -lallegro_acodec-debug -lallegro_image-debug -lallegro_font-debug -lallegro_ttf-debug -DDEBUG -std=gnu99 -g -Wall && ./bin/superderpy
