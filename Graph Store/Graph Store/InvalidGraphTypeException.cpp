#include "stdafx.h"
#include "InvalidGraphTypeException.h"

InvalidGraphTypeException::InvalidGraphTypeException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

InvalidGraphTypeException::InvalidGraphTypeException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
