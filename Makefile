.PHONY: all build
	# Always run these rules

.SUFFIXES:
	# Disable default rules

SOURCES = $(wildcard src/*.cpp)
HEADER = $(wildcard src/*.h)

GPP := g++

CPPFLAGS := -iquote src -Wall -g -std=c++11
LDFLAGS := -static -static-libgcc -static-libstdc++ -lVirtDisk -luuid

all: build

build: $(SOURCES)
	$(GPP) $(CPPFLAGS) $^ $(LDFLAGS) -o VHDTool.exe
