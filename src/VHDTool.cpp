#include "stdafx.h"

#include "Util.h"
#include "VHDTool.h"

using namespace std;

namespace VHDTool {

	// OperationName
	OperationName::OperationName(const string name, const string description, Operation operation,
			const initializer_list<const OptionName> options):
		name(name),
		description(description),
		operation(operation),
		options(options)
	{}

	const string OperationName::getName(void) const {
		return name;
	}
	const string OperationName::getDescription(void) const {
		return description;
	}
	Operation OperationName::getOperationValue(void) const {
		return operation;
	}
	const initializer_list<const OptionName> OperationName::getOptions(void) const {
		return options;
	}

	// OptionName

	OptionName::OptionName(const string name, const string shortForm, const string description,
			Option option, bool caseSensitive):
		name(name),
		shortForm(shortForm),
		description(description),
		option(option),
		caseSensitive(caseSensitive)
	{}

	const string OptionName::getName(void) const {
		return name;
	}
	const string OptionName::getShortForm(void) const {
		return shortForm;
	}
	const string OptionName::getDescription(void) const {
		return description;
	}
	Option OptionName::getOptionValue(void) const {
		return option;
	}
	bool OptionName::isCaseSensitive(void) const {
		return caseSensitive;
	}

	// FileOptions

	FileOptions::FileOptions(void):
		readOnly(true),
		extension()
	{}

	FileOptions::FileOptions(const bool readOnly, const string extension):
		readOnly(readOnly),
		extension(extension)
	{}

	bool FileOptions::isReadOnly(void) const {
		return readOnly;
	}
	void FileOptions::setReadOnly(bool readOnly) {
		this->readOnly = readOnly;
	}
	const string FileOptions::getExtension(void) const {
		return extension;
	}
	void FileOptions::setExtension(const string extension) {
		if (!extension.empty()) {
			this->extension = extension;
		} else {
			this->extension = EXTENSION_UNDEFINED;
		}
	}

	// ProgramOptions

	ProgramOptions::ProgramOptions():
		fileOptions(),
		recursive(false),
		tryAllFiles(false)
	{}

	void ProgramOptions::setRecursive(const bool recursive) {
		this->recursive = recursive;
	}
	bool ProgramOptions::getRecursive(void) const {
		return recursive;
	}
	void ProgramOptions::setTryAllFiles(const bool tryAllFiles) {
		this->tryAllFiles = tryAllFiles;
	}
	bool ProgramOptions::getTryAllFiles(void) const {
		return tryAllFiles;
	}
	FileOptions& ProgramOptions::getFileOptions() {
		return fileOptions;
	}

	// Supported options

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

	const initializer_list<const OptionName> OPTIONS_HELP = {};
	const OperationName OPERATION_HELP = OperationName(
		"help", "Prints this help text",
		Operation::Help,
		OPTIONS_HELP
	);
	const initializer_list<const OptionName> OPTIONS_MOUNT = {
		OPTION_DIRECTORY,
		OPTION_DIRECTORY_RECURSIVE,
		OPTION_TRYALL,
		OPTION_READONLY
	};
	const OperationName OPERATION_MOUNT = OperationName(
		"mount", "Mounts a specified file / files in a directory",
		Operation::Mount,
		OPTIONS_MOUNT
	);
	const initializer_list<const OptionName> OPTIONS_DISMOUNT = {
		OPTION_DIRECTORY,
		OPTION_DIRECTORY_RECURSIVE,
		OPTION_TRYALL
	};
	const OperationName OPERATION_DISMOUNT = OperationName(
		"unmount", "Dismounts a specified file / files in a directory",
		Operation::Dismount,
		OPTIONS_DISMOUNT
	);

	const initializer_list<const OperationName> OPERATIONS = {
		OPERATION_HELP,
		OPERATION_MOUNT,
		OPERATION_DISMOUNT
	};
	const initializer_list<const OperationName> getSupportedOperations(void) {
		return OPERATIONS;
	}

	// Supported extensions

	const initializer_list<const string> EXTENSIONS = {
		EXTENSION_VHD,
		EXTENSION_VHDX,
		EXTENSION_ISO
	};
	const std::initializer_list<const std::string> getSupportedExtensions(void) {
		return EXTENSIONS;
	}

};
