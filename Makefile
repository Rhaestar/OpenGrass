#************************************************
#*                                              *
#*             (c) 2017 J. FABRIZIO             *
#*                                              *
#*                               LRDE EPITA     *
#*                                              *
#************************************************

CC = g++

CPP_FILES = src/program.cc src/plane.cc
HXX_FILES =
OBJ_FILES = $(CPP_FILES:.cc=.o)

CXX_FLAGS = -Wall -Wextra -Ofast -g -std=c++11 -I include/
CXX_FLAGS += $(shell sdl2-config --cflags)
CXX_FLAGS += #-m64 -march=native
CXX_FLAGS += #-fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize

LDXX_FLAGS = -lGL -lGLEW -lglut $(shell sdl2-config --libs) -lSDL2_image

DIST = main
MAIN_FILE=src/main.o

all: build

build: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

%.o : %.cc
	$(CC) -c -o $@ $< $(CXX_FLAGS) $(LDXX_FLAGS)

.PHONY: all clean build

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DIST)
