// Copyright Tobias Faller 2016

#pragma once

//#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#include <SDKDDKVer.h>

#include <iostream>
#include <string>
#include <vector>
#include <codecvt>

#include <boost\system\windows_error.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\locale.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment(lib,"uuid.lib")
#include <initguid.h>

#pragma comment(lib, "VirtDisk.lib")
#include <virtdisk.h>
