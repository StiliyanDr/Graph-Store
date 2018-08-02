#include "stdafx.h"
#include "FileParser.h"
#include <exception>
#include <string>

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

FileParser::FileParser(const char* fileName) :
	FileParser()
{
	openFile(fileName);
}

void FileParser::openFile(const char* fileName)
{
	if (hasOpenedAFile())
	{
		closeFile();
	}

	file.open(fileName);
	throwExceptionIfFailedToOpen(fileName);
	lineNumber = 1;
}

bool FileParser::hasOpenedAFile() const
{
	return file.is_open();
}

void FileParser::closeFile()
{
	if (hasOpenedAFile())
	{
		file.close();
		lineNumber = 0;
	}
}

void FileParser::throwExceptionIfFailedToOpen(const String& fileName) const
{
	if (!hasOpenedAFile())
	{
		throw std::exception("Could not open file for reading, name: " + fileName);
	}
}

String FileParser::readLine()
{
	assertValidState();

	file.getline(buffer, BUFFER_SIZE);
	throwExceptionIfInErrorState("Empty line!");
	++lineNumber;

	return buffer;
}

void FileParser::assertValidState() const
{
	assert(hasOpenedAFile());
	assert(!hasReachedEnd());
	assert(!file.fail());
}

bool FileParser::hasReachedEnd() const
{
	assert(hasOpenedAFile());

	return file.eof();
}

void FileParser::throwExceptionIfInErrorState(const char* message) const
{
	if (file.fail())
	{
		std::string suffix = " Error at line " + std::to_string(lineNumber);
		throw std::runtime_error(message + suffix);
	}
}

void FileParser::skipUntil(char symbol)
{
	assertValidState();

	char extractedSymbol;

	do
	{
		extractedSymbol = file.get();

		if (extractedSymbol == NEW_LINE_SYMBOL)
		{
			++lineNumber;
		}
	} while (!hasReachedEnd() && extractedSymbol != symbol);
}

char FileParser::peek()
{
	assertValidState();

	return file.peek();
}

unsigned FileParser::parseUnsigned()
{
	return parseNumber<unsigned>();
}

char FileParser::endOfFileSymbol()
{
	return EOF;
}
