#include "GraphBuilderException.h"

GraphBuilderException::GraphBuilderException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

GraphBuilderException::GraphBuilderException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
