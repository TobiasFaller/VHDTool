#pragma once

using namespace std;

namespace VHDTool {

	void PrintError(const DWORD errorCode);

	const bool CheckFileExtension(const string path, const string extension);
	const string CheckOneOfFileExtensions(const string path, const vector<string> extensions);
	const string ConcatPath(const string path, const string file);
	const string GetFileName(const string path);
	const string GetFileExtension(const string path);
	const string GetFilePath(const string path);

	const wstring toWChar(const string text);
	const string fromWChar(const wstring text);
}