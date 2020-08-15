.PHONY: all build
	# Always run these rules

.SUFFIXES:
	# Disable default rules

SOURCES = $(wildcard src/*.cpp)
HEADER = $(wildcard src/*.h)

CXX := g++
STRIP := strip

CXXFLAGS := -iquote src -Wall -g -std=c++11
LDFLAGS := -static -static-libgcc -static-libstdc++ -lVirtDisk -luuid
STRIPFLAGS := --strip-unneeded

all: build

build: $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o VHDTool.exe
	$(STRIP) $(STRIPFLAGS) VHDTool.exe
