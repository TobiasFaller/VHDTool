#include "stdafx.h"

#include "Util.h"
#include "VHDTool.h"

using namespace std;

namespace VHDTool {

	int Run(vector<string> arguments);
	void PrintUsage(const string path);
	string StripLeadingDashes(const string argument);

	void DoActionOnSingleFile(const string path, Operation operation, FileOptions arguments);
	void DoActionOnDirectory(const string path, Operation operation, ProgramOptions arguments);
	void DoActionOnFiles(const string path, Operation operation, ProgramOptions arguments);

	void PrintUsage(const string programmName) {
		const string fileName = GetFileName(programmName);

		cout << "Usage: " << fileName << " <operation> [options] <path> path path ..." << endl;
		cout << endl;
		cout << "Operations:" << endl;
		for (const OperationName& operation : GetSupportedOperations()) {
			cout << "  " << operation.getName() << endl;
			cout << "    " << operation.getDescription() << endl << endl;

			for (const OptionName& option : operation.getOptions()) {
				cout << "    --" << option.getName() << "(-" << option.getShortForm() << ")" << endl;
				cout << "      " << option.getDescription() << endl << endl;
			}
		}
	}

	int Run(vector<string> arguments)
	{
		cout << "VHDTool v1.0" << endl;
		cout << endl;

		if (arguments.size() < 2) {
			PrintUsage(move(arguments[0]));
			return EXIT_FAILURE;
		}

		const string opName = StripLeadingDashes(arguments[1]);
		const OperationName* operation = nullptr;
		for (const OperationName& operationName : GetSupportedOperations()) {
			if (opName == operationName.getName()) {
				operation = &operationName;
				break;
			}
		}

		if (operation == nullptr) {
			cout << "Unknown operation \"" << opName << "\"!" << endl << endl;
			PrintUsage(arguments[0]);
			return EXIT_FAILURE;
		}

		size_t options = (size_t) Option::None;
		size_t argument = 2;
		for (; argument + 1 < arguments.size(); argument++){
			string argumentValue;
			bool shortArg = false;

			if (arguments[argument].find("--") == 0) {
				argumentValue = arguments[argument].substr(2);
			}
			else if (arguments[argument].find("-") == 0){
				shortArg = true;
				argumentValue = arguments[argument].substr(1);
			}
			else {
				break; // Interpret as Path
			}

			bool validArg = false;
			for (const OptionName& optionName : operation->getOptions()) {
				const string optionNameText = shortArg ? optionName.getShortForm() : optionName.getName();

				if (optionName.isCaseSensitive()
					? boost::equals(argumentValue, optionNameText)
					: boost::iequals(argumentValue, optionNameText)) {

					if ((options & (size_t) optionName.getOptionValue()) != 0) {
						cout << "Option \"" << argumentValue << "\" already specified! Ignoring second one!"
							<< endl;
						break;
					}

					validArg = true;
					options |= (size_t) optionName.getOptionValue();
					break;
				}
			}

			if (!validArg) {
				cout << "Unknown option \"" << argumentValue << "\"!" << endl;
				cout << endl;
				PrintUsage(arguments[0]);
				return EXIT_FAILURE;
			}
		}

		if (operation->getOperationValue() == Operation::Help) {
			PrintUsage(arguments[0]);
			return EXIT_SUCCESS;
		}

		if (arguments.size() - argument <= 0) {
			cout << "No path/file specified!" << endl;
			cout << endl;
			PrintUsage(arguments[0]);
			return EXIT_FAILURE;
		}

		ProgramOptions programOptions;
		programOptions.getFileOptions().setReadOnly((options & (size_t) Option::ReadOnly) != 0);
		programOptions.setTryAllFiles((options & (size_t) Option::TryAll) != 0);
		programOptions.setRecursive((options & (size_t) Option::DirectoryRecursive) != 0);

		const bool paramIsDirectory = ((options & ((size_t) Option::Directory | (size_t) Option::DirectoryRecursive)) != 0);
		void(*function)(const string, Operation, ProgramOptions)
			= (paramIsDirectory ? &DoActionOnDirectory : &DoActionOnFiles);

		for (; argument < arguments.size(); argument++) {
			cout << "Mounting " << arguments[argument] << " as " << (paramIsDirectory ? "directory" : "file/s") << "!" << endl;

			function(arguments[argument], operation->getOperationValue(), programOptions);
		}

		return EXIT_SUCCESS;
	}

	void DoActionOnSingleFile(const string path, Operation operation, FileOptions arguments) {
		VIRTUAL_STORAGE_TYPE storageType;
		HANDLE handle;
		DWORD error;
		VIRTUAL_DISK_ACCESS_MASK diskAccessMask;

		storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;
		storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;

#ifdef VIRTUAL_STORAGE_TYPE_DEVICE_ISO
		if (boost::iequals(arguments.getType(), FILETYPE_ISO)) {
			storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_ISO;
		}
#endif

#ifdef VIRTUAL_STORAGE_TYPE_DEVICE_VHDX
		if (boost::iequals(arguments.getType(), FILETYPE_VHDX)) {
			storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHDX;
		}
#endif

		switch (operation) {
		case VHDTool::Operation::Mount:
			diskAccessMask = arguments.isReadOnly() ? VIRTUAL_DISK_ACCESS_ATTACH_RO : VIRTUAL_DISK_ACCESS_ATTACH_RW;
			break;
		case VHDTool::Operation::Dismount:
			diskAccessMask = VIRTUAL_DISK_ACCESS_DETACH;
			break;
		default:
			diskAccessMask = VIRTUAL_DISK_ACCESS_ALL;
			break;
		}

		wstring pathW = toWChar(path);
		error = OpenVirtualDisk(&storageType, pathW.c_str(), diskAccessMask, OPEN_VIRTUAL_DISK_FLAG_NONE, NULL,
			&handle);
		pathW.clear();

		if (error) {
			cout << "Could not open " << arguments.getType() << " file \"" << path << "\"" << endl;
			PrintError(error);
			return;
		}

		switch (operation) {
		case VHDTool::Operation::Mount: {
			underlying_type<ATTACH_VIRTUAL_DISK_FLAG>::type attachFlags = ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME;
			if (arguments.isReadOnly()) {
				attachFlags |= ATTACH_VIRTUAL_DISK_FLAG_READ_ONLY;
			}

			error = AttachVirtualDisk(handle, NULL, (ATTACH_VIRTUAL_DISK_FLAG) attachFlags, 0, NULL, NULL);
			if (error) {
				cout << "Could not attach " << arguments.getType() << " file \"" << path << "\"" << endl;
				PrintError(error);
				return;
			}
			cout << arguments.getType() << " file \"" << path << "\" attached!" << endl;
			break;
		}
		case VHDTool::Operation::Dismount: {
			error = DetachVirtualDisk(handle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
			if (error) {
				cout << "Could not detach " << arguments.getType() << " file \"" << path << "\"" << endl;
				PrintError(error);
				return;
			}
			cout << arguments.getType() << " file \"" << path << "\" detached!" << endl;
			break;
		}
		default:
			break;
		}
	}

	void DoActionOnFiles(const string path, Operation operation, ProgramOptions arguments) {
		HANDLE handleFind;
		WIN32_FIND_DATAW findData;

		wstring pathW = toWChar(path);
		handleFind = FindFirstFileW(pathW.c_str(), &findData);
		pathW.clear();

		if (handleFind == INVALID_HANDLE_VALUE) {
			cout << "Could not find file/s!" << endl;
			PrintError(GetLastError());
			return;
		}

		const string parentDirectory = GetFilePath(path);
		do {
			if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				continue;
			}

			const string fileName = fromWChar(findData.cFileName);
			const string fileType = GetFileType(fileName);
			if (fileType != FILETYPE_UNDEFINED || arguments.getTryAllFiles()) {
				const string newPath = ConcatPath(parentDirectory, fileName);
				if (newPath.empty()) {
					cout << "Path \"" << parentDirectory << "\" is too long to mount file \"" << fileName << "\"!" << endl;
					continue;
				}

				FileOptions fileOptions(arguments.getFileOptions());
				fileOptions.setType(fileType);
				DoActionOnSingleFile(newPath, operation, fileOptions);
			}
		} while (FindNextFileW(handleFind, &findData) != 0);

		FindClose(handleFind);
	}

	void DoActionOnDirectory(const string path, Operation operation, ProgramOptions arguments) {
		HANDLE handleFind;
		WIN32_FIND_DATAW findData;

		const string searchString = ConcatPath(path, "*");
		if (searchString.empty()) {
			cout << "Path \"" << path << "\" is too long to search for files!" << endl;
			return;
		}

		wstring searchStringW = toWChar(searchString);
		handleFind = FindFirstFileW(searchStringW.c_str(), &findData);
		searchStringW.clear();

		if (handleFind == INVALID_HANDLE_VALUE) {
			cout << "Could not list directory contents / no files!" << endl;
			PrintError(GetLastError());
			return;
		}

		do {
			const string fileName = fromWChar(wstring(findData.cFileName));

			if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				if (arguments.getRecursive()) {
					if (fileName == "." || fileName == "..") {
						continue;
					}

					const string newPath = ConcatPath(path, fileName);
					if (newPath.empty()) {
						cout << "Path \"" << fileName << "\" is too long to search for files!" << endl;
						continue;
					}

					DoActionOnDirectory(newPath, operation, arguments);
				}
				continue;
			}

			const string fileType = GetFileType(fileName);
			if (fileType != FILETYPE_UNDEFINED || arguments.getTryAllFiles()) {
				const string newPath = ConcatPath(path, fileName);
				if (newPath.empty()) {
					cout << "Path \"" << path << "\" is too long to mount file!";
					continue;
				}

				FileOptions fileOptions(arguments.getFileOptions());
				fileOptions.setType(fileType);
				DoActionOnSingleFile(newPath, operation, fileOptions);
			}
		} while (FindNextFileW(handleFind, &findData) != 0);

		FindClose(handleFind);
	}

	string StripLeadingDashes(const string argument) {
		// Starts with --
		if (argument.find("--") == 0) {
			return argument.substr(2);
		}

		// Starts with -
		if (argument.find("-") == 0) {
			return argument.substr(1);
		}

		return argument;
	}

};

int main(const int argc, const char* argv[]) {
	vector<string> parsedParams = vector<string>();

	for (int arg = 0; arg < argc; arg++) {
		parsedParams.push_back(string(argv[arg]));
	}

	return VHDTool::Run(parsedParams);
}
