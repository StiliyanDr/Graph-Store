#ifndef __FILE_PARSER_EXCEPTION_HEADER_INCLUDED__
#define __FILE_PARSER_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class FileParserException : public std::runtime_error
{
public:
	explicit FileParserException(const char* errorMessage) :
		runtime_error(errorMessage)
	{
	}

	explicit FileParserException(const std::string& errorMessage) :
		runtime_error(errorMessage)
	{
	}
};

#endif //__FILE_PARSER_EXCEPTION_HEADER_INCLUDED__