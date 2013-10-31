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

LIBS = -lpthread $(shell sdl-config --libs) -lSDL_mixer
CFLAGS_RELEASE = -O3 -ffast-math
CFLAGS = -Wall ${CFLAGS_RELEASE}
CFLAGS_DEBUG = -g
SDLINCLUDE := $(shell sdl-config --cflags)

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifeq ($(uname_S),Linux)
    OPENGLINCLUDE = -I/usr/include/GL
    LIBS += -lGL -lGLU
endif
ifeq ($(uname_S),Darwin)
    # CFLAGS += -mmacosx-version-min=10.8
    OPENGLINCLUDE = -I/System/Library/Frameworks/OpenGL.framework/Headers
    LIBS += -framework Cocoa -framework OpenGL
    depends += SDLMain.o
endif
ifneq (,$(findstring MINGW,$(uname_S)))
    SDLMIXINCLUDE =
    BASEINCLUDE = -I/mingw/include
    SDLINCLUDE = $(BASEINCLUDE)/SDL
    OPENGLINCLUDE = $(BASEINCLUDE)/GL
    LDINCLUDE = -L/mingw/lib
    LIBS += -lopengl32 -lglu32
    depends += WinMain.o
endif

INCLUDE = $(BASEINCLUDE) $(SDLINCLUDE) $(OPENGLINCLUDE)

$(target):$(depends)
	@echo
	@echo Making standard target: $(target)
	$(CPPC) -o $(target) $(depends) $(LDFLAGS) $(LDINCLUDE) $(LIBS)

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
