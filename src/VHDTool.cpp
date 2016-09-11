#include "stdafx.h"

#include "Util.h"
#include "VHDTool.h"

using namespace std;

namespace VHDTool {

	// OperationName

	OperationName::OperationName() : name(), description(), operation(), options(), valid(false) {}
	OperationName::OperationName(const char* name, const char* description, const Operation operation,
		const vector<const OptionName> options) : name(name), description(description), operation(operation),
		options(options), valid(true) {}

	OperationName& OperationName::operator= (const OperationName& other) {
		if (this != &other) {
			valid = other.valid;
			name = other.name;
			description = other.description;
			operation = other.operation;
			options = other.options;
		}

		return *this;
	}

	const bool OperationName::isValid() {
		return valid;
	}
	const string OperationName::getName() {
		return name;
	}
	const string OperationName::getDescription() {
		return description;
	}
	const Operation OperationName::getOperationValue() {
		return operation;
	}
	const vector<const OptionName>::const_iterator OperationName::begin() {
		return options.cbegin();
	}
	const vector<const OptionName>::const_iterator OperationName::end() {
		return options.cend();
	}

	// OptionName

	OptionName::OptionName() : name(), shortForm(), description(), option(), caseSensitive(), valid(false) {}
	OptionName::OptionName(const char* name, const char* shortForm, const char* description,
		const Option option, const bool caseSensitive) : name(name), shortForm(shortForm),
		description(description), option(option), caseSensitive(caseSensitive), valid(true) {}

	OptionName& OptionName::operator= (const OptionName& other) {
		if (this != &other) {
			valid = other.valid;
			name = other.name;
			shortForm = other.shortForm;
			description = other.description;
			option = other.option;
			caseSensitive = other.caseSensitive;
		}

		return *this;
	}

	const bool OptionName::isValid() {
		return valid;
	}
	const string OptionName::getName() {
		return name;
	}
	const string OptionName::getShortForm() {
		return shortForm;
	}
	const string OptionName::getDescription() {
		return description;
	}
	const Option OptionName::getOptionValue() {
		return option;
	}
	const bool OptionName::isCaseSensitive() {
		return caseSensitive;
	}

	// FileOptions

	FileOptions::FileOptions() : readOnly(true), extension() {}
	FileOptions::FileOptions(const bool readOnly, const string extension) : readOnly(readOnly), extension(extension) {}

	const bool FileOptions::isReadOnly() {
		return readOnly;
	}
	void FileOptions::setReadOnly(const bool readOnly) {
		this->readOnly = readOnly;
	}
	const string FileOptions::getExtension() {
		return extension;
	}
	const void FileOptions::setExtension(const string extension) {
		this->extension = extension;
	}

	// ProgramOptions

	ProgramOptions::ProgramOptions() : fileOptions(), recursive(false), tryAllFiles(false) {}
	ProgramOptions::ProgramOptions(FileOptions fileOptions, const bool recursive, const bool tryAllFiles)
		: fileOptions(fileOptions), recursive(recursive), tryAllFiles(tryAllFiles) {}

	void ProgramOptions::setRecursive(const bool recursive) {
		this->recursive = recursive;
	}
	const bool ProgramOptions::getRecursive() {
		return recursive;
	}
	void ProgramOptions::setTryAllFiles(const bool tryAllFiles) {
		this->tryAllFiles = tryAllFiles;
	}
	const bool ProgramOptions::getTryAllFiles() {
		return tryAllFiles;
	}
	FileOptions* ProgramOptions::getFileOptions() {
		return &(this->fileOptions);
	}
}