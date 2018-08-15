#ifndef __MISSING_ARGUMENT_EXCEPTION_HEADER_INCLUDED__
#define __MISSING_ARGUMENT_EXCEPTION_HEADER_INCLUDED__

#include "../Command Exception/CommandException.h"

class MissingArgumentException : public CommandException
{
public:
	explicit MissingArgumentException(const char* argumentName);
	explicit MissingArgumentException(const std::string& argumentName);
	virtual ~MissingArgumentException() = default;

private:
	static std::string format(const std::string& argumentName);
};

#endif //__MISSING_ARGUMENT_EXCEPTION_HEADER_INCLUDED__