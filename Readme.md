VHD Tool
===================

[![Build Status](https://travis-ci.org/TobiasFaller/VHDTool.svg?branch=master)](https://travis-ci.org/TobiasFaller/VHDTool)

This utility provides VHD / VHDX (un-)mount support using the Microsoft (c) virtual-disk API.
It's meant as fast replacement for DiskPart providing the same functionality.
Using diskpart (un-)mounting disk images took (Windows 7 / 8.1) a lot of time ~1 sec per file.
So i wrote this small utility to make (un-)mounting faster.

Requirements
-------------

- Visual Studio
- C++11
- Boost library
