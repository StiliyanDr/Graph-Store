#include "MissingArgumentException.h"

MissingArgumentException::MissingArgumentException(const char* argumentName) :
	MissingArgumentException(std::string(argumentName))
{
}

MissingArgumentException::MissingArgumentException(const std::string& argumentName) :
	CommandException(format(argumentName))
{
}

std::string MissingArgumentException::format(const std::string& argumentName)
{
	return "Missing argument: [" + argumentName + "]!";
}
