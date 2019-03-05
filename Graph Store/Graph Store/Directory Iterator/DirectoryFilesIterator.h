#ifndef __DIRECTORY_FILES_ITERATOR_HEADER_INCLUDED__
#define __DIRECTORY_FILES_ITERATOR_HEADER_INCLUDED__

#include <windows.h>
#include "../String/String.h"

class DirectoryFilesIterator
{
	typedef HANDLE SearchHandle;
	typedef WIN32_FIND_DATA File;

public:
	DirectoryFilesIterator();
	explicit DirectoryFilesIterator(const String& path);
	DirectoryFilesIterator(const DirectoryFilesIterator&) = delete;
	DirectoryFilesIterator& operator=(const DirectoryFilesIterator&) = delete;
	DirectoryFilesIterator(DirectoryFilesIterator&&) = delete;
	DirectoryFilesIterator& operator=(DirectoryFilesIterator&&) = delete;
	~DirectoryFilesIterator();

	void startIterationIn(const String& path);
	void advance();
	bool isValid() const;
	String getNameOfCurrentFile() const;
	String getPathOfCurrentFile() const;

private:
	void endIteration();
	void invalidateHandle();
	void setPath(const String& path);
	String getPathWithAppendedPattern() const;
	void throwExceptionIfFailedToOpenDirectory(const String& path) const;
	bool failedToOpenDirectory() const;

private:
	static const String pattern;

private:
	String path;
	SearchHandle handle;
	File file;
};

#endif //__DIRECTORY_FILES_ITERATOR_HEADER_INCLUDED__