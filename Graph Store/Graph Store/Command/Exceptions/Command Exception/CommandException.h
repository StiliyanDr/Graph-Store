#ifndef __COMMAND_EXCEPTION_HEADER_INCLUDED__
#define __COMMAND_EXCEPTION_HEADER_INCLUDED__

#include "Runtime Error\RuntimeError.h"

class CommandException : public RuntimeError
{
public:
	explicit CommandException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit CommandException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__COMMAND_EXCEPTION_HEADER_INCLUDED__