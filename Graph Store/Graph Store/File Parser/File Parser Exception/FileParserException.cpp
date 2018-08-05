#include "FileParserException.h"

FileParserException::FileParserException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

FileParserException::FileParserException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
