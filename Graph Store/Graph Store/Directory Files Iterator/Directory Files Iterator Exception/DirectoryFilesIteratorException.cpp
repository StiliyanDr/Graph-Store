#include "DirectoryFilesIteratorException.h"

DirectoryFilesIteratorException::DirectoryFilesIteratorException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

DirectoryFilesIteratorException::DirectoryFilesIteratorException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
