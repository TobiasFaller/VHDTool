#pragma once

#include "stdafx.h"

namespace VHDTool {

	void PrintError(const DWORD errorCode);

	bool CheckFileExtension(const std::string path, const std::string extension);
	const std::string ConcatPath(const std::string path, const std::string file);
	const std::string GetFileName(const std::string path);
	const std::string GetFileExtension(const std::string path);
	const std::string GetFilePath(const std::string path);

	const std::wstring toWChar(const std::string text);
	const std::string fromWChar(const std::wstring text);

};
