IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -Wall -ansi -g

SRCDIR=src
ODIR=obj
LIBS=-lallegro -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_font -lallegro_main -lallegro_ttf -lallegro_primitives -lm

_OBJ = config.o main.o about.o intro.o loading.o map.o menu.o level.o moonwalk.o pause.o timeline.o allegro_utils.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUTPUTDIR = bin

$(ODIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUTDIR)/superderpy: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $(OUTPUTDIR)/superderpy

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ allegro.log $(SRCDIR)/*~ $(OUTPUTDIR)/*
