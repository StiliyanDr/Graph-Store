#include "FileParser.h"
#include "File Parser/Open File Fail Exception/OpenFileFailException.h"
#include "String/String.h"
#include <string>
#include <assert.h>

FileParser::FileParser() :
	lineNumber(0)
{
}

FileParser::FileParser(FileParser&& source) :
	lineNumber(source.lineNumber),
	file(std::move(source.file))
{
	source.lineNumber = 0;
}

FileParser& FileParser::operator=(FileParser&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

void FileParser::swapContentsWith(FileParser parser)
{
	std::swap(lineNumber, parser.lineNumber);
	std::swap(file, parser.file);
}

FileParser::FileParser(const String& fileName) :
	FileParser()
{
	openFile(fileName);
}

void FileParser::openFile(const String& fileName)
{
	if (hasOpenedFile())
	{
		closeFile();
	}

	file.open(fileName.cString());
	verifySuccessfulOpeningOf(fileName);
	lineNumber = 1;
}

bool FileParser::hasOpenedFile() const
{
	return file.is_open();
}

void FileParser::closeFile()
{
	if (hasOpenedFile())
	{
		file.close();
		lineNumber = 0;
	}
}

void FileParser::verifySuccessfulOpeningOf(const String& fileName) const
{
	if (!hasOpenedFile())
	{
		throw OpenFileFailException("Could not open file for reading, name: " + fileName);
	}
}

String FileParser::readLine()
{
	getLine();
	
	return buffer;
}

void FileParser::getLine()
{
	verifyValidState();

	file.getline(buffer, BUFFER_SIZE);
	throwExceptionIfInErrorState("No more characters left in the file!");
	++lineNumber;
}

void FileParser::verifyValidState() const
{
	verifyAFileIsOpened();
	verifyEndIsNotReached();
	verifyNoPreviousOperationFailed();
}

void FileParser::verifyAFileIsOpened() const
{
	if (!hasOpenedFile())
	{
		throw FileParserException("No file is currently opened!"_s);
	}
}

void FileParser::verifyEndIsNotReached() const
{
	if (hasReachedEnd())
	{
		throw FileParserException("End of file already reached!"_s);
	}
}

void FileParser::verifyNoPreviousOperationFailed() const
{
	if (file.fail())
	{
		throw FileParserException("A previous operation already failed!"_s);
	}
}

bool FileParser::hasReachedEnd() const
{
	verifyAFileIsOpened();

	return file.eof();
}

void FileParser::throwExceptionIfInErrorState(const char* message) const
{
	assert(message != nullptr);

	if (file.fail())
	{
		std::string suffix = " Error at line " + std::to_string(lineNumber);
		throw FileParserException(message + suffix);
	}
}

String FileParser::readAndTrimLine()
{
	getLine();
	trimEnd();

	return trimStartAndReturnPointer();
}

void FileParser::trimEnd()
{
	int end = strlen(buffer) - 1;

	while (end >= 0 && buffer[end] == ' ')
	{
		--end;
	}

	buffer[end + 1] = '\0';
}

const char*
FileParser::trimStartAndReturnPointer() const
{
	const char* p = buffer;

	while (*p == ' ')
	{
		++p;
	}

	return p;
}

void FileParser::skipUntil(char character)
{
	verifyValidState();

	char extractedCharacter;

	do
	{
		extractedCharacter = file.get();

		if (extractedCharacter == '\n')
		{
			++lineNumber;
		}
	} while (!(hasReachedEnd() || extractedCharacter == character));
}

void FileParser::skipSpaces()
{
	verifyValidState();

	while (peek() == ' ')
	{
		file.ignore();
	}
}

char FileParser::peek()
{
	verifyValidState();

	return file.peek();
}

unsigned FileParser::parseUnsigned()
{
	skipSpaces();
	verifyNumberIsNonNegative();

	return parseNumber<unsigned>();
}

void FileParser::verifyNumberIsNonNegative()
{
	if (peek() == '-')
	{
		file.setstate(file.rdstate() | std::ios::failbit);
	}

	throwExceptionIfInErrorState("Number must not be negative!");
}

char FileParser::endOfFileCharacter()
{
	return EOF;
}
