#pragma once

#include "stdafx.h"

namespace VHDTool {

	// Enums

	enum class Option: size_t {
		None = 0x00,
		TryAll = (1 << 0x01),
		ReadOnly = (1 << 0x02),
		Directory = (1 << 0x03),
		DirectoryRecursive = (1 << 0x04)
	};
	enum class Operation: size_t {
		Mount,
		Dismount,
		Help
	};

	// Classes

	class OperationName;
	class OptionName;
	class FileOptions;
	class ProgramOptions;

	class OperationName {
		const std::string name;
		const std::string description;
		const Operation operation;
		const std::vector<OptionName> options;

	public:
		OperationName(const std::string name, const std::string description,
			Operation operation, const std::initializer_list<OptionName> options);

		const std::string getName(void) const;
		const std::string getDescription(void) const;
		Operation getOperationValue(void) const;
		const std::vector<OptionName>& getOptions(void) const;
	};

	class OptionName {
	private:
		std::string name;
		std::string shortForm;
		std::string description;
		Option option;
		bool caseSensitive;

	public:
		OptionName(const std::string name, const std::string shortForm,
			const std::string description, Option option, bool caseSensitive);

		const std::string getName(void) const;
		const std::string getShortForm(void) const;
		const std::string getDescription(void) const;
		Option getOptionValue(void) const;
		bool isCaseSensitive(void) const;
	};

	class FileOptions {
	private:
		bool readOnly;
		std::string extension;

	public:
		FileOptions(void);
		FileOptions(bool readOnly, const std::string extension);

		bool isReadOnly() const;
		void setReadOnly(bool readOnly);
		const std::string getExtension() const;
		void setExtension(const std::string extension);
	};

	class ProgramOptions {
	private:
		FileOptions fileOptions;
		bool recursive;
		bool tryAllFiles;

	public:
		ProgramOptions(void);

		void setRecursive(const bool recursive);
		bool getRecursive(void) const;
		void setTryAllFiles(const bool tryAllFiles);
		bool getTryAllFiles(void) const;
		FileOptions& getFileOptions(void);
	};

	// Constants

	const std::string EXTENSION_UNDEFINED = std::string("Undefined");
	const std::string EXTENSION_VHD = std::string("VHD");
	const std::string EXTENSION_VHDX = std::string("VHDX");
	const std::vector<std::string> EXTENSIONS = {EXTENSION_VHD, EXTENSION_VHDX};

	const std::string SEARCH_APPEND = std::string("*");

	const std::string DIRECTORY_SAME = std::string(".");
	const std::string DIRECTORY_PARENT = std::string("..");

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

	const OperationName OPERATION_HELP = OperationName(
		"help", "Prints this help text",
		Operation::Help,
		{}
	);
	const OperationName OPERATION_MOUNT = OperationName(
		"mount", "Mounts a specified file / files in a directory",
		Operation::Mount,
		{
			OPTION_DIRECTORY,
			OPTION_DIRECTORY_RECURSIVE,
			OPTION_TRYALL,
			OPTION_READONLY
		}
	);
	const OperationName OPERATION_DISMOUNT = OperationName(
		"unmount", "Dismounts a specified file / files in a directory",
		Operation::Dismount,
		{
			OPTION_DIRECTORY,
			OPTION_DIRECTORY_RECURSIVE,
			OPTION_TRYALL
		}
	);

	const std::initializer_list<OperationName> OPERATION_NAMES = {
		OPERATION_HELP,
		OPERATION_MOUNT,
		OPERATION_DISMOUNT
	};
};
