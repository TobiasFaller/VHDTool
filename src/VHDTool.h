#pragma once

using namespace std;

namespace VHDTool {

	// Enums

	enum Option {
		None = 0x00,
		TryAll = (1 << 0x01),
		ReadOnly = (1 << 0x02),
		Directory = (1 << 0x03),
		DirectoryRecursive = (1 << 0x04)
	};
	enum Operation {
		Mount,
		Dismount,
		Help,
		InvalidArg,
		InvalidOperation
	};

	// Classes

	class FileOptions {
	private:
		bool readOnly;
		string extension;

	public:
		FileOptions();
		FileOptions(bool readOnly, const string extension);

		const bool isReadOnly();
		void setReadOnly(const bool readOnly);
		const string getExtension();
		const void setExtension(const string extension);
	};

	class ProgramOptions {
	private:
		FileOptions fileOptions;

		bool recursive;
		bool tryAllFiles;

	public:
		ProgramOptions();
		ProgramOptions(FileOptions fileOptions, const bool recursive, const bool tryAllFiles);

		void setRecursive(const bool recursive);
		const bool getRecursive();
		void setTryAllFiles(const bool tryAllFiles);
		const bool getTryAllFiles();

		FileOptions* getFileOptions();
	};

	class OptionName {
	private:
		bool valid;
		string name;
		string shortForm;
		string description;
		Option option;
		bool caseSensitive;

	public:
		OptionName();
		OptionName(const char* name, const char* shortForm,
			const char* description, Option option,
			const bool caseSensitive);

		OptionName& operator= (const OptionName& other);

		const bool isValid();
		const string getName();
		const string getShortForm();
		const string getDescription();
		const Option getOptionValue();
		const bool isCaseSensitive();
	};

	class OperationName {
		bool valid;
		string name;
		string description;
		Operation operation;
		vector<OptionName> options;

	public:
		OperationName();
		OperationName(const char* name, const char* description,
			Operation operation, const vector<OptionName> options);

		OperationName& operator= (const OperationName& other);

		const bool isValid();
		const string getName();
		const string getDescription();
		const Operation getOperationValue();
		const vector<OptionName>::const_iterator begin();
		const vector<OptionName>::const_iterator end();
	};

	// Constants

	const string EXTENSION_UNDEFINED = string("Undefined");
	const string EXTENSION_VHD = string("VHD");
	const string EXTENSION_VHDX = string("VHDX");
	const vector<string> EXTENSIONS = {EXTENSION_VHD, EXTENSION_VHDX};

	const string SEARCH_APPEND = string("*");

	const string DIRECTORY_SAME = string(".");
	const string DIRECTORY_PARENT = string("..");

	const OptionName OPTION_TRYALL = OptionName(
		"all", "a", "Mount also files with other extension than \"vhd\" or \"vhdx\"",
		Option::TryAll, false
	);
	const OptionName OPTION_READONLY = OptionName(
		"readonly", "r", "Mounts the file in read-only mode",
		Option::ReadOnly, false
	);
	const OptionName OPTION_DIRECTORY = OptionName(
		"directory", "d", "Applies the operation to all files in the specified directory/ies.",
		Option::Directory, true
	);
	const OptionName OPTION_DIRECTORY_RECURSIVE = OptionName(
		"Directory", "D", "Applies the operation to all files in the specified directory/ies and sub-directories.",
		Option::DirectoryRecursive, true
	);

	const vector<OptionName> MOUNT_OPTIONS = {
		OPTION_DIRECTORY,
		OPTION_DIRECTORY_RECURSIVE,
		OPTION_TRYALL,
		OPTION_READONLY
	};

	const vector<OptionName> DISMOUNT_OPTIONS = {
		OPTION_DIRECTORY,
		OPTION_DIRECTORY_RECURSIVE,
		OPTION_TRYALL
	};

	const OperationName OPERATION_HELP = OperationName(
		"help", "Prints this help text",
		Operation::Help, vector<OptionName>()
	);
	const OperationName OPERATION_MOUNT = OperationName(
		"mount", "Mounts a specified file / files in a directory",
		Operation::Mount, MOUNT_OPTIONS
	);
	const OperationName OPERATION_DISMOUNT = OperationName(
		"unmount", "Dismounts a specified file / files in a directory",
		Operation::Dismount, DISMOUNT_OPTIONS
	);

	const vector<OperationName> OPERATION_NAMES = {
		OPERATION_HELP,
		OPERATION_MOUNT,
		OPERATION_DISMOUNT
	};
}