#include "stdafx.h"

#include "Util.h"
#include "VHDTool.h"

using namespace std;

namespace VHDTool {

	// OperationName
	OperationName::OperationName(const string name, const string description, Operation operation,
			const initializer_list<OptionName> options):
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
	const vector<OptionName>& OperationName::getOptions(void) const {
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
		this->extension = extension;
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
};
