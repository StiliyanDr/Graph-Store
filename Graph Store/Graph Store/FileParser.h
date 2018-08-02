#ifndef __FILE_PARSER_HEADER_INCLUDED__
#define __FILE_PARSER_HEADER_INCLUDED__

#include <fstream>
#include "String.h"

class FileParser
{
public:
	static char endOfFileSymbol();

public:
	FileParser();
	explicit FileParser(const char* fileName);
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;
	FileParser(FileParser&& source);
	FileParser& operator=(FileParser&& rhs);
	~FileParser() = default;

	void openFile(const char* fileName);
	void closeFile();
	bool hasReachedEnd() const;
	bool hasOpenedAFile() const;

	String readLine();
	unsigned parseUnsigned();
	char peek();
	void skipUntil(char symbol);

private:
	template <class Number>
	Number parseNumber();
	
	void throwExceptionIfFailedToOpen(const String& fileName) const;
	void throwExceptionIfInErrorState(const char* message) const;
	void assertValidState() const;
	void swapContentsWith(FileParser parser);

private:
	static const size_t BUFFER_SIZE = 512;
	static const char NEW_LINE_SYMBOL = '\n';

private:
	unsigned lineNumber;
	char buffer[BUFFER_SIZE];
	std::ifstream file;
};

#include "FileParser.hpp"

#endif //__FILE_PARSER_HEADER_INCLUDED__