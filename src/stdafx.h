// Copyright Tobias Faller 2016

#pragma once

//#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#include <SDKDDKVer.h>

#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <codecvt>
#include <cstdlib>

#include <boost\system\windows_error.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\locale.hpp>
#include <boost\format.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef _MSC_VER
#pragma comment(lib,"uuid.lib")
#endif
#include <initguid.h>

#ifdef _MSC_VER
#pragma comment(lib, "VirtDisk.lib")
#endif
#include <virtdisk.h>
