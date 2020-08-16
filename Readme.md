VHD Tool
===================

[![Build Status](https://travis-ci.org/TobiasFaller/VHDTool.svg?branch=master)](https://travis-ci.org/TobiasFaller/VHDTool)

This utility provides VHD / VHDX (un-)mount support using the Microsoft (c) virtual-disk API.
It's meant as fast replacement for DiskPart providing the same functionality.
Using diskpart (un-)mounting disk images took (Windows 7 / 8.1) a lot of time ~1 sec per file.
So i wrote this small utility to make (un-)mounting faster.

Build Requirements
------------------

- Visual Studio
- C++11
- Boost library

Tool Usage
----------

Below is an overview of the tools arguments:

```text
Usage: VHDTool.exe  <operation> [options] <path> path path ...
Operations:
  mount: Mounts one or more VHD or VHDX files specified by path
    -r: Mount the images as read-only
    -d: Mounts all images in the directory specified by the path
    -D: Mounts all images in the directory specified by the path
        and all subdirectories
    -a: Tries to mount all files even if they have not the correct
        file extension
  unmount: Unmounts one or more VHD or VHDX files specified by path
    -d: Unmounts all images in the directory specified by the path
    -D: Unmounts all images in the directory specified by the path
        and all subdirectories
    -a: Tries to unmount all files even if they have not the correct
        file extension
```

Here are some examples how to use this tool:

```bat
REM mount a single VHD / VHDX file
VHDTool.exe mount SomeDrive.vhd

REM mount a single VHD / VHDX file as read-only
VHDTool.exe mount -r SomeDrive.vhd

REM mount all VHD / VHDX files in the folder
VHDTool.exe mount -d SomeFolder

REM mount all VHD / VHDX files in the folder and all subfolders
VHDTool.exe mount -d SomeFolder

REM unmount all VHD / VHDX files in the folder and all subfolders
VHDTool.exe unmount -d SomeFolder
```
