#ifndef __OPEN_FILE_FAIL_EXCEPTION_HEADER_INCLUDED__
#define __OPEN_FILE_FAIL_EXCEPTION_HEADER_INCLUDED__

#include "../File Parser Exception/FileParserException.h"

class OpenFileFailException : public FileParserException
{
public:
	explicit OpenFileFailException(const char* errorMessage) :
		FileParserException(errorMessage)
	{
	}

	explicit OpenFileFailException(const std::string& errorMessage) :
		FileParserException(errorMessage)
	{
	}
};

#endif //__OPEN_FILE_FAIL_EXCEPTION_HEADER_INCLUDED__