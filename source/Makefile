#This is the makefile for Crystal Quest 3D

CC = gcc
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
CFLAGS = -Wall
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -O3 -ffast-math

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifeq ($(uname_S),Linux)
    SDLINCLUDE = -I/usr/include/SDL
    SDLMIXINCLUDE = $(SDLINCLUDE)
    OPENGLINCLUDE = -I/usr/include/GL
    LIBS += -lSDL -lGL -lGLU -lSDL_mixer
endif
ifeq ($(uname_S),Darwin)
    SDLINCLUDE = -I/Library/Frameworks/SDL.framework/Headers
    SDLMIXINCLUDE = -I/Library/Frameworks/SDL_mixer.framework/Headers
    OPENGLINCLUDE = -I/System/Library/Frameworks/OpenGL.framework/Headers
    LIBS += -framework SDL -framework SDL_mixer -framework Cocoa -framework OpenGL
    depends += SDLMain.o
endif
ifneq (,$(findstring MINGW,$(uname_S)))
    SDLINCLUDE = -I/mingw/include/SDL
    SDLMIXINCLUDE = $(SDLINCLUDE)
    BASEINCLUDE = -I/mingw/include
    INCBASE = /c/Program\ Files/Microsoft\ SDKs/Windows/v6.1/Include
    OPENGLINCLUDE = -I$(INCBASE) -I$(INCBASE)/GL
    LDINCLUDE = -L/c/usr/lib
    LIBS += -lSDL -lopengl32 -lglu32 -lSDL_mixer
    LDFLAGS +=
    CFLAGS +=
    depends += WinMain.o
endif

INCLUDE = $(BASEINCLUDE) $(SDLINCLUDE) $(SDLMIXINCLUDE) $(OPENGLINCLUDE)

$(target):$(depends)
	@echo
	@echo Making standard target: $(target)
	$(CPPC) -o $(target) $(depends) $(CFLAGS) $(LDFLAGS) $(LDINCLUDE) $(LIBS)

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

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	@echo Cleaning up...
	@rm -f $(target) $(depends)
	@rm -f *~
	@echo done
