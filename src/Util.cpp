#include "stdafx.h"

#include "Util.h"

using namespace std;

namespace VHDTool {

	const string DIRECTORY_SEPARATORS = string("/\\");
	const string DIRECTORY_SEPARATOR = string("\\");
	const string EXTENSION_SEPARATOR = string(".");

	const bool CheckFileExtension(const string path, const string extension) {
		return boost::algorithm::iequals(GetFileExtension(GetFileName(path)).c_str(), extension.c_str());
	}

	const string CheckOneOfFileExtensions(const string path, const vector<string> extensions) {
		for (const string extension : extensions) {
			if (CheckFileExtension(path, extension)) {
				return extension;
			}
		}

		return string();
	}

	const string GetFileName(const string path) {
		size_t position = path.find_last_of(DIRECTORY_SEPARATORS.c_str());
		if (position == string::npos) {
			return path;
		}

		return path.substr(position + 1);
	}

	const string GetFileExtension(const string path) {
		size_t position = path.find_last_of(EXTENSION_SEPARATOR.c_str());
		if (position == string::npos) {
			return path;
		}

		return path.substr(position + 1);
	}

	const string GetFilePath(const string path) {
		size_t position = path.find_last_of(DIRECTORY_SEPARATORS.c_str());
		if (position == string::npos) {
			return path;
		}

		return path.substr(0, position);
	}

	void PrintError(const DWORD error) {
		wchar_t* errorText;
		if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&errorText, 0, NULL) != 0) {
			string message = fromWChar(wstring(errorText));

			//wstring_convert<codecvt_utf8<wchar_t>> utf8conv;
			//string data = utf8conv.to_bytes(wstring((wchar_t*)errorText));
			//string message = boost::locale::conv::from_utf((wchar_t*)errorText, locale());
			cout << "Error " << boost::format("0x%08x") % error << ": " << message << endl;

			LocalFree(errorText);
		}
		else {
			cout << "Error: " << boost::format("0x%08x") % error << endl;
		}
	}

	const string ConcatPath(const string path, const string file) {
		if (path.length() + file.length() + 2 > MAX_PATH) {
			return string();
		}

		string str = string(path.c_str());
		str.append(DIRECTORY_SEPARATOR.c_str());
		str.append(file.c_str());

		return str;
	}

	const string fromWChar(const wstring text) {
		const wchar_t* textData = text.c_str();
		mbstate_t mbstate = mbstate_t();

		// Get size of resulting string
		size_t retVal = wcsrtombs(nullptr, &textData, 0, &mbstate);
		if (retVal == static_cast<std::size_t>(-1)) {
			cout << "Error: Could not convert error into current codepage!" << endl;
			return string();
		}

		char* message = (char*)malloc(sizeof(char) * (retVal + 1));
		if (message == nullptr) {
			cout << "Error: Could not allocate space for error-message!" << endl;
			return string();
		}

		// Do conversion
		retVal = wcsrtombs(message, &textData, (retVal + 1), &mbstate);
		if (retVal == static_cast<std::size_t>(-1)) {
			cout << "Error: Could not convert error into current codepage!" << endl;
			return string();
		}

		string finalText = string(message);
		free(message);
		return finalText;
	}

	const wstring toWChar(const string text) {
		const char* textData = text.c_str();
		mbstate_t mbstate = mbstate_t();

		// Get size of resulting string
		size_t retVal = mbsrtowcs(nullptr, &textData, 0, &mbstate);
		if (retVal == static_cast<std::size_t>(-1)) {
			cout << "Error: Could not convert error into current codepage!" << endl;
			return wstring();
		}

		wchar_t* message = (wchar_t*) malloc(sizeof(wchar_t) * (retVal + 1));
		if (message == nullptr) {
			cout << "Error: Could not allocate space for error-message!" << endl;
			return wstring();
		}

		// Do conversion
		retVal = mbsrtowcs(message, &textData, (retVal + 1), &mbstate);
		if (retVal == static_cast<std::size_t>(-1)) {
			cout << "Error: Could not convert error into current codepage!" << endl;
			return wstring();
		}

		wstring finalText = wstring(message);
		free(message);
		return finalText;
	}
};
