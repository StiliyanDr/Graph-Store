#include "stdafx.h"
#include "OpenFileFailException.h"

OpenFileFailException::OpenFileFailException(const char* errorMessage) :
	FileParserException(errorMessage)
{
}

OpenFileFailException::OpenFileFailException(const std::string& errorMessage) :
	FileParserException(errorMessage)
{
}
