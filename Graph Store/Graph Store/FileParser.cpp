#include "stdafx.h"
#include "FileParser.h"
#include <exception>

FileParser::FileParser() :
	lineNumber(0)
{
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
