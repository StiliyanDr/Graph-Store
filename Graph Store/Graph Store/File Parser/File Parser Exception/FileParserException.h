#ifndef __FILE_PARSER_EXCEPTION_HEADER_INCLUDED__
#define __FILE_PARSER_EXCEPTION_HEADER_INCLUDED__

#include "../../Runtime Error/RuntimeError.h"

class FileParserException : public RuntimeError
{
public:
	explicit FileParserException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit FileParserException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__FILE_PARSER_EXCEPTION_HEADER_INCLUDED__