IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -std=gnu99 -Wall -DDEBUG -g

SRCDIR=src
ODIR=obj
LIBS=-lallegro -lallegro_audio-debug -lallegro_acodec-debug -lallegro_image-debug -lallegro_font-debug -lallegro_ttf-debug -lm

_OBJ = main.o about.o intro.o loading.o map.o menu.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

bin/superderpy: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ allegro.log $(SRCDIR)/*~ 
