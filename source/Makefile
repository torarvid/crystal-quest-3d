#This is the makefile for Crystal Quest 3D

CC = g++
CPPC = g++
target = ../cq3d
depends = pak_util.o \
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

LIBS = -lpthread

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifeq ($(uname_S),Linux)
    SDLINCLUDE = /usr/include/SDL
    SDLMIXINCLUDE = $(SDLINCLUDE)
    OPENGLINCLUDE = /usr/include/GL
    LIBS += -lSDL -lGL -lGLU -lSDL_mixer
endif
ifeq ($(uname_S),Darwin)
    SDLINCLUDE = /Library/Frameworks/SDL.framework/Headers
    SDLMIXINCLUDE = /Library/Frameworks/SDL_mixer.framework/Headers
    OPENGLINCLUDE = /System/Library/Frameworks/OpenGL.framework/Headers
    LIBS += -framework SDL -framework SDL_mixer -framework Cocoa -framework OpenGL
    depends += SDLMain.o
endif

INCLUDE = -I$(SDLINCLUDE) -I$(SDLMIXINCLUDE) -I$(OPENGLINCLUDE)
CFLAGS = -Wall
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -O3 -ffast-math

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
