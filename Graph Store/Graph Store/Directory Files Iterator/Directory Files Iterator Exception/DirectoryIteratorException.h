#ifndef __DIRECTORY_ITERATOR_EXCEPTION_HEADER_INCLUDED__
#define __DIRECTORY_ITERATOR_EXCEPTION_HEADER_INCLUDED__

#include "../../Runtime Error/RuntimeError.h"

class DirectoryIteratorException : public RuntimeError
{
public:
	explicit DirectoryIteratorException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit DirectoryIteratorException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__DIRECTORY_ITERATOR_EXCEPTION_HEADER_INCLUDED__