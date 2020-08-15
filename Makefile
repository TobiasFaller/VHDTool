.PHONY: all build
	# Always run these rules

.SUFFIXES:
	# Disable default rules

SOURCES = $(wildcard src/*.cpp)
HEADER = $(wildcard src/*.h)

CXX := g++
STRIP := strip
WINDRES := windres

CXXFLAGS := -iquote src -Wall -g -std=c++11
LDFLAGS := -static -static-libgcc -static-libstdc++ -lVirtDisk -luuid
STRIPFLAGS := --strip-unneeded
WINDRESFLAGS :=

all: build

build: $(SOURCES)
	$(WINDRES) $(WINDRESFLAGS) src/vhdtool.rc src/vhdtool.rc.o
	$(CXX) $(CXXFLAGS) src/vhdtool.rc.o $^ $(LDFLAGS) -o VHDTool.exe
	$(STRIP) $(STRIPFLAGS) VHDTool.exe
