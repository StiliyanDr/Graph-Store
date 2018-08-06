#include "DirectoryFilesIterator.h"
#include <assert.h>
#include <stdexcept>

const String DirectoryFilesIterator::pattern = "*.txt";

DirectoryFilesIterator::DirectoryFilesIterator()
{
	invalidateHandle();
}

void DirectoryFilesIterator::invalidateHandle()
{
	handle = INVALID_HANDLE_VALUE;
}

DirectoryFilesIterator::DirectoryFilesIterator(const String& path) :
	DirectoryFilesIterator()
{
	startIterationIn(path);
}

void DirectoryFilesIterator::startIterationIn(const String& path)
{
	if (isValid())
	{
		endIteration();
	}

	String pathWithPattern = appendPatternTo(path);
	handle = FindFirstFile(pathWithPattern, &file);
	throwExceptionIfFailedToOpenDirectory(path);
}

bool DirectoryFilesIterator::isValid() const
{
	return handle != INVALID_HANDLE_VALUE;
}

void DirectoryFilesIterator::endIteration()
{
	assert(isValid());

	FindClose(handle);
	invalidateHandle();
}

String DirectoryFilesIterator::appendPatternTo(const String& path)
{
	return path + '\\' + pattern;
}

void DirectoryFilesIterator::throwExceptionIfFailedToOpenDirectory(const String& path) const
{
	if (failedToOpenDirectory())
	{
		throw std::runtime_error("Could not open directory: " + path);
	}
}

bool DirectoryFilesIterator::failedToOpenDirectory() const
{
	return !isValid() && GetLastError() != ERROR_FILE_NOT_FOUND;
}

DirectoryFilesIterator::~DirectoryFilesIterator()
{
	if (isValid())
	{
		endIteration();
	}
}

void DirectoryFilesIterator::advance()
{
	if (!isValid())
	{
		return;
	}

	bool foundAFile = FindNextFile(handle, &file);

	if (!foundAFile)
	{
		endIteration();
	}
}

String DirectoryFilesIterator::getNameOfCurrentFile() const
{
	assert(isValid());

	return file.cFileName;
}
