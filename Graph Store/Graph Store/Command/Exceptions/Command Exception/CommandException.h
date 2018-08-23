#ifndef __COMMAND_EXCEPTION_HEADER_INCLUDED__
#define __COMMAND_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class CommandException : public std::runtime_error
{
public:
	explicit CommandException(const char* errorMessage) :
		runtime_error(errorMessage)
	{
	}

	explicit CommandException(const std::string& errorMessage) :
		runtime_error(errorMessage)
	{
	}
};

#endif //__COMMAND_EXCEPTION_HEADER_INCLUDED__