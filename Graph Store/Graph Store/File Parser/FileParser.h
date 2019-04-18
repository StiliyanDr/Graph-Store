#ifndef __FILE_PARSER_HEADER_INCLUDED__
#define __FILE_PARSER_HEADER_INCLUDED__

#include <fstream>

class String;

class FileParser
{
public:
	static char endOfFileCharacter();

public:
	FileParser();
	explicit FileParser(const String& fileName);
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;
	FileParser(FileParser&& source);
	FileParser& operator=(FileParser&& rhs);
	~FileParser() = default;

	void openFile(const String& fileName);
	void closeFile();
	bool hasReachedEnd() const;
	bool hasOpenedFile() const;

	String readLine();
	unsigned parseUnsigned();
	char peek();
	void skipUntil(char character);
	void skipSpaces();

private:
	template <class Number>
	Number parseNumber();

	void verifySuccessfulOpeningOf(const String& fileName) const;
	void verifyValidState() const;
	void verifyAFileIsOpened() const;
	void verifyEndIsNotReached() const;
	void verifyNoPreviousOperationFailed() const;
	void verifyNumberIsNonNegative();
	void throwExceptionIfInErrorState(const char* message) const;
	void swapContentsWith(FileParser parser);

private:
	static const size_t BUFFER_SIZE = 512;

private:
	unsigned lineNumber;
	char buffer[BUFFER_SIZE];
	std::ifstream file;
};

template <class Number>
Number FileParser::parseNumber()
{
	Number number;
	file >> number;

	throwExceptionIfInErrorState("Invalid number format!");

	return number;
}

#endif //__FILE_PARSER_HEADER_INCLUDED__