#This is the makefile for Crystal Quest 3D

CC = g++-2.95
CPPC = g++-2.95
target = ../cq3d
depends = 	pak_util.o \
		Vector.o \
		bitmap.o \
		common.o \
		Model3ds.o \
		init.o \
		event.o \
		draw.o \
		particle.o \
		collision.o \
		main.o

CFLAGS = -Wall `sdl-config --cflags`
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -O3 -ffast-math
#LIBS = -lpthread -lnet -lpcap
LIBS = -lpthread -lGL -lGLU `sdl-config --libs` -lSDL_mixer

$(target):$(depends)
	@echo
	@echo Making standard target: $(target)
	$(CPPC) -o $(target) $(depends) $(CFLAGS) $(LIBS)

debug:$(depends)
	@echo
	@echo Making debug target: $(target)
	$(CPPC) -o $(target) $(depends) $(CFLAGS) $(CFLAGS_DEBUG) $(LIBS)

opt:$(depends)
	@echo
	@echo Making optimized target: $(target)
	$(CPPC) -o $(target) $(depends) $(CFLAGS) $(CFLAGS_RELEASE) $(LIBS)

.cpp.o:
	$(CPPC) $(CFLAGS) -c $<
#	$(CPPC) $(CFLAGS) -g -c $<

.c.o:
	$(CC) $(CFLAGS) -c $<
#	$(CC) $(CFLAGS) -g -c $<

clean:
	@echo Cleaning up...
	@rm -f $(target) $(depends)
	@rm -f *~
	@echo done
