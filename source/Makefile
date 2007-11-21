#This is the makefile for Crystal Quest 3D

CC = g++
CPPC = g++
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
		main.o \
		SDLMain.o

INCLUDE = -I/Library/Frameworks/SDL.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers -I/Library/Frameworks/SDL_mixer.framework/Headers
CFLAGS = -Wall
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -O3 -ffast-math
#LIBS = -lpthread -lnet -lpcap
LIBS = -lpthread -framework SDL -framework SDL_mixer -framework Cocoa -framework OpenGL

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

.m.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

.cpp.o:
	$(CPPC) $(CFLAGS) $(INCLUDE) -c $<
#	$(CPPC) $(CFLAGS) -g -c $<

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<
#	$(CC) $(CFLAGS) -g -c $<

clean:
	@echo Cleaning up...
	@rm -f $(target) $(depends)
	@rm -f *~
	@echo done
