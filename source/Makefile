#This is the makefile for Crystal Quest 3D

CC = gcc
target = cq3d
depends = 	init.o \
		event.o \
		draw.o \
		main.o

CFLAGS = -Wall -ansi
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -O3 -fomit-frame-pointer
#LIBS = -lpthread -lnet -lpcap
LIBS = -lGL -lGLU `sdl-config --cflags --libs`

$(target):$(depends)
	@echo
	@echo Making standard target: $(target)
	$(CC) -o $(target) $(depends) $(CFLAGS) $(LIBS)

debug:$(depends)
	@echo
	@echo Making debug target: $(target)
	$(CC) -o $(target) $(depends) $(CFLAGS) $(CFLAGS_DEBUG) $(LIBS)

opt:$(depends)
	@echo
	@echo Making optimized target: $(target)
	$(CC) -o $(target) $(depends) $(CFLAGS) $(CFLAGS_RELEASE) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	@echo Cleaning up...
	@rm -f $(target) $(depends)
	@rm -f *~
	@echo done
