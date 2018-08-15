#ifndef __COMMAND_EXCEPTION_HEADER_INCLUDED__
#define __COMMAND_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class CommandException : public std::runtime_error
{
public:
	explicit CommandException(const char* errorMessage);
	explicit CommandException(const std::string& errorMessage);
	virtual ~CommandException() = default;
};

#endif //__COMMAND_EXCEPTION_HEADER_INCLUDED__