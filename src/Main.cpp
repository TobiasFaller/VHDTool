#include "stdafx.h"

#include "Util.h"
#include "VHDTool.h"

using namespace std;

namespace VHDTool {

	int Run(vector<string> arguments);
	void PrintUsage(const string path);

	void DoActionOnSingleFile(const string path, Operation operation, FileOptions arguments);
	void DoActionOnDirectory(const string path, Operation operation, ProgramOptions arguments);
	void DoActionOnFiles(const string path, Operation operation, ProgramOptions arguments);

	void PrintUsage(const string programmName) {
		const string fileName = GetFileName(programmName);

		cout << "Usage: " << fileName << " <operation> [options] <path> path path ..." << endl;
		cout << endl;
		cout << "Operations:" << endl;
		for (const OperationName& operation : OPERATION_NAMES) {
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
			return 1;
		}

		const string opName = arguments[1].substr((arguments[1].find("--") == 0)
			? 2 : ((arguments[1].find("-") == 0) ? 1 : 0));

		const OperationName* operation = nullptr;
		for (const OperationName& operationName : OPERATION_NAMES) {
			if (opName == operationName.getName()) {
				operation = &operationName;
				break;
			}
		}

		if (operation == nullptr) {
			cout << "Unknown operation \"" << opName << "\"!" << endl << endl;
			PrintUsage(arguments[0]);
			return 1;
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
				return 1;
			}
		}

		if (operation->getOperationValue() == Operation::Help) {
			PrintUsage(arguments[0]);
			return 0;
		}

		if (arguments.size() - argument <= 0) {
			cout << "No path/file specified!" << endl;
			cout << endl;
			PrintUsage(arguments[0]);
			return 1;
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

		return 0;
	}

	void DoActionOnSingleFile(const string path, Operation operation, FileOptions arguments) {
		VIRTUAL_STORAGE_TYPE storageType;
		HANDLE handle;
		DWORD error;
		VIRTUAL_DISK_ACCESS_MASK diskAccessMask;

		if (arguments.getExtension().empty()) {
			arguments.setExtension(CheckOneOfFileExtensions(path, EXTENSIONS));
		}

#if _WIN32_WINNT > _WIN32_WINNT_WIN8
		bool isVhdx = boost::iequals(arguments.getExtension(), EXTENSION_VHDX);
#endif
		const string readableExtension = (!arguments.getExtension().empty() ? arguments.getExtension() : EXTENSION_UNDEFINED);

		storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;
#if _WIN32_WINNT > _WIN32_WINNT_WIN8
		storageType.DeviceId = isVhdx ? VIRTUAL_STORAGE_TYPE_DEVICE_VHDX : VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
#else
		storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
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
			cout << "Could not open " << readableExtension << " file \"" << path << "\"" << endl;
			PrintError(error);
			return;
		}

		switch (operation) {
		case VHDTool::Operation::Mount:
			error = AttachVirtualDisk(handle, NULL,
				(ATTACH_VIRTUAL_DISK_FLAG)(ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME
					| (arguments.isReadOnly() ? ATTACH_VIRTUAL_DISK_FLAG_READ_ONLY : ATTACH_VIRTUAL_DISK_FLAG_NONE)),
				0, NULL, NULL);
			if (error) {
				cout << "Could not attach " << readableExtension << " file \"" << path << "\"" << endl;
				PrintError(error);
				return;
			}
			cout << readableExtension << " file \"" << path << "\" attached!" << endl;
			break;
		case VHDTool::Operation::Dismount:
			error = DetachVirtualDisk(handle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
			if (error) {
				cout << "Could not detach " << readableExtension << " file \"" << path << "\"" << endl;
				PrintError(error);
				return;
			}
			cout << readableExtension << " file \"" << path << "\" detached!" << endl;
			break;
		default:
			diskAccessMask = VIRTUAL_DISK_ACCESS_ALL;
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
			const string extension = CheckOneOfFileExtensions(fileName, EXTENSIONS);
			if (!extension.empty() || arguments.getTryAllFiles()) {
				const string newPath = ConcatPath(parentDirectory, fileName);
				if (newPath.empty()) {
					cout << "Path \"" << parentDirectory << "\" is too long to mount file \"" << fileName
						<< "\"!" << endl;
					continue;
				}

				// Create copy of file options
				FileOptions fileOptions = arguments.getFileOptions();
				fileOptions.setExtension(extension);
				DoActionOnSingleFile(newPath, operation, fileOptions);
			}
		} while (FindNextFileW(handleFind, &findData) != 0);

		FindClose(handleFind);
	}

	void DoActionOnDirectory(const string path, Operation operation, ProgramOptions arguments) {
		HANDLE handleFind;
		WIN32_FIND_DATAW findData;

		const string searchString = ConcatPath(path, SEARCH_APPEND);
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
					if ((boost::algorithm::equals(fileName.c_str(), DIRECTORY_SAME.c_str()) == 0)
						|| (boost::algorithm::equals(fileName.c_str(), DIRECTORY_PARENT.c_str()) == 0)) {
						continue;
					}

					const string newPath = ConcatPath(path, fileName);
					if (newPath.empty()) {
						cout << "Path \"" << fileName.c_str() << "\" is too long to search for files!" << endl;
						continue;
					}

					DoActionOnDirectory(newPath, operation, arguments);
				}
				continue;
			}

			const string extension = CheckOneOfFileExtensions(fileName, EXTENSIONS);
			if (!extension.empty() || arguments.getTryAllFiles()) {
				const string newPath = ConcatPath(path, fileName);
				if (newPath.empty()) {
					cout << "Path \"" << path << "\" is too long to mount file!";
					continue;
				}

				// Create copy of file options
				FileOptions fileOptions = arguments.getFileOptions();
				fileOptions.setExtension(extension);
				DoActionOnSingleFile(newPath, operation, fileOptions);
			}
		} while (FindNextFileW(handleFind, &findData) != 0);

		FindClose(handleFind);
	}
};

int main(const int argc, const char* argv[]) {
	vector<string> parsedParams = vector<string>();

	for (int arg = 0; arg < argc; arg++) {
		parsedParams.push_back(string(argv[arg]));
	}

	return VHDTool::Run(parsedParams);
}
