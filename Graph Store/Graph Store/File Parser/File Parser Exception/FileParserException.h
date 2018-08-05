#ifndef __FILE_PARSER_EXCEPTION_HEADER_INCLUDED__
#define __FILE_PARSER_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class FileParserException : public std::runtime_error
{
public:
	explicit FileParserException(const char* errorMessage);
	explicit FileParserException(const std::string& errorMessage);
	virtual ~FileParserException() = default;
};

#endif //__FILE_PARSER_EXCEPTION_HEADER_INCLUDED__