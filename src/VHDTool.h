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
		const std::initializer_list<const OptionName>& options;

	public:
		OperationName(const std::string name, const std::string description,
			Operation operation, const std::initializer_list<const OptionName>& options);

		const std::string getName(void) const;
		const std::string getDescription(void) const;
		Operation getOperationValue(void) const;
		const std::initializer_list<const OptionName>& getOptions(void) const;
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
		std::string type;

	public:
		FileOptions(void);
		FileOptions(bool readOnly, const std::string type);

		bool isReadOnly() const;
		void setReadOnly(bool readOnly);
		const std::string getType() const;
		void setType(const std::string type);
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

	const std::initializer_list<const OperationName>& GetSupportedOperations(void);

	// Constants

	const std::string FILETYPE_UNDEFINED = "Undefined";
	const std::string FILETYPE_VHD = "VHD";
	const std::string FILETYPE_VHDX = "VHDX";
	const std::string FILETYPE_ISO = "ISO";

	const std::string GetFileType(const std::string path);

};
