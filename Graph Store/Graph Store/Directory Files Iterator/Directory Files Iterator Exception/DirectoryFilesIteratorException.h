#ifndef __DIRECTORY_FILES_ITERATOR_EXCEPTION_HEADER_INCLUDED__
#define __DIRECTORY_FILES_ITERATOR_EXCEPTION_HEADER_INCLUDED__

#include "../../Runtime Error/RuntimeError.h"

class DirectoryFilesIteratorException : public RuntimeError
{
public:
	explicit DirectoryFilesIteratorException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit DirectoryFilesIteratorException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__DIRECTORY_FILES_ITERATOR_EXCEPTION_HEADER_INCLUDED__