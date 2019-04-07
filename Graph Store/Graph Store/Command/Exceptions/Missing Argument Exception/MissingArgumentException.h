#ifndef __MISSING_ARGUMENT_EXCEPTION_HEADER_INCLUDED__
#define __MISSING_ARGUMENT_EXCEPTION_HEADER_INCLUDED__

#include "../Command Exception/CommandException.h"

class MissingArgumentException : public CommandException
{
public:
	explicit MissingArgumentException(const std::string& argumentName) :
		CommandException(format(argumentName))
	{
	}
	
private:
	static std::string format(const std::string& argumentName)
	{
		return "Missing argument: [" + argumentName + "]!";
	}
};

#endif //__MISSING_ARGUMENT_EXCEPTION_HEADER_INCLUDED__