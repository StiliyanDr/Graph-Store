#ifndef __DIRECTORY_FILES_ITERATOR_EXCEPTION_HEADER_INCLUDED__
#define __DIRECTORY_FILES_ITERATOR_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class DirectoryFilesIteratorException : public std::runtime_error
{
public:
	explicit DirectoryFilesIteratorException(const char* errorMessage);
	explicit DirectoryFilesIteratorException(const std::string& errorMessage);
	virtual ~DirectoryFilesIteratorException() = default;
};

#endif //__DIRECTORY_FILES_ITERATOR_EXCEPTION_HEADER_INCLUDED__