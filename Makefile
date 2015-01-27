CC=g++

CFLAGS1 = -c -Wall
CFLAGS2 = $(shell sdl-config --cflags)
CFLAGS3 = $(shell pkg-config --cflags --libs opencv)
LDLIBS= -Wall $(shell sdl-config --libs) -lSDL_net -lSDL_image -lpthread -lcv -lhighgui

all: hmincv

hmincv: main.o image.o orfilter.o cvimage.o layer.o 
	$(CC) $(LDLIBS) main.o image.o orfilter.o cvimage.o layer.o -o hmincv

main.o: main.cpp
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) main.cpp

image.o: image.cpp
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) image.cpp

orfilter.o: orfilter.cpp
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) orfilter.cpp

cvimage.o: cvimage.cpp
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) cvimage.cpp

layer.o: layer.cpp
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) layer.cpp


clean:
	rm -rf *o *gch hmincv

