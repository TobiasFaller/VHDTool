.PHONY: all build
	# Always run these rules

.SUFFIXES:
	# Disable default rules

SOURCES = $(wildcard src/*.cpp)
HEADER = $(wildcard src/*.h)

CXX := g++
CXXFLAGS := -iquote src -Wall -g -std=c++11
LDFLAGS := -static -static-libgcc -static-libstdc++ -lVirtDisk -luuid

all: build

build: $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o VHDTool.exe
