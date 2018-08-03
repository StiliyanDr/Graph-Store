#ifndef __OPEN_FILE_FAIL_EXCEPTION_HEADER_INCLUDED__
#define __OPEN_FILE_FAIL_EXCEPTION_HEADER_INCLUDED__

#include "FileParserException.h"

class OpenFileFailException : public FileParserException
{
public:
	explicit OpenFileFailException(const char* errorMessage);
	explicit OpenFileFailException(const std::string& errorMessage);
	virtual ~OpenFileFailException() = default;
};

#endif //__OPEN_FILE_FAIL_EXCEPTION_HEADER_INCLUDED__