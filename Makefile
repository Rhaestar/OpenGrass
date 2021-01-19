#************************************************
#*                                              *
#*             (c) 2017 J. FABRIZIO             *
#*                                              *
#*                               LRDE EPITA     *
#*                                              *
#************************************************

CC = g++

CPP_FILES = src/program.cc src/main.cc
HXX_FILES =
OBJ_FILES = $(CPP_FILES:.cc=.o)

CXX_FLAGS = -Wall -Wextra -Ofast -g -std=c++11 -I include/ -fopenmp
CXX_FLAGS +=
CXX_FLAGS += -m64 -march=native
CXX_FLAGS += -fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize

LDXX_FLAGS = -lGL -lGLEW -lglut

DIST = main

all: build

build: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

%.o : %.cc
	$(CC) -c -o $@ $< $(CXX_FLAGS) $(LDXX_FLAGS)

.PHONY: all clean build

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DIST)
