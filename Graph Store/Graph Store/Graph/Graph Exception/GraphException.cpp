#include "GraphException.h"

GraphException::GraphException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

GraphException::GraphException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
