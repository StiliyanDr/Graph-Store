#include "stdafx.h"
#include "UnsupportedAlgorithmException.h"

UnsupportedAlgorithmException::UnsupportedAlgorithmException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

UnsupportedAlgorithmException::UnsupportedAlgorithmException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
