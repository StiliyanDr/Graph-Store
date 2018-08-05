#include "InvalidGraphTypeException.h"

InvalidGraphTypeException::InvalidGraphTypeException(const char* errorMessage) :
	GraphException(errorMessage)
{
}

InvalidGraphTypeException::InvalidGraphTypeException(const std::string& errorMessage) :
	GraphException(errorMessage)
{
}
