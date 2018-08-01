#ifndef __FILE_PARSER_HEADER_INCLUDED__
#define __FILE_PARSER_HEADER_INCLUDED__

#include <fstream>
#include "String.h"

class FileParser
{
public:
	FileParser();
	FileParser(const char* fileName);
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;
	FileParser(FileParser&&);
	FileParser& operator=(FileParser&&);
	~FileParser() = default;

	void openFile(const char* fileName);
	void closeFile();
	bool hasReachedEnd() const;
	bool hasOpenedAFile() const;

	String readLine();
	unsigned parseUnsigned();
	void skipUntil(char symbol);

private:
	template <class Number>
	Number parseNumber();
	
	void throwExceptionIfFailedToOpen(const String& fileName) const;
	void throwExceptionIfInErrorState(const char* message) const;
	void assertValidState() const;

private:
	static const size_t BUFFER_SIZE = 512;

private:
	unsigned lineNumber;
	char buffer[BUFFER_SIZE];
	std::ifstream file;
};

#include "FileParser.hpp"

#endif //__FILE_PARSER_HEADER_INCLUDED__