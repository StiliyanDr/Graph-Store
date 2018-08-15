#include "CommandException.h"

CommandException::CommandException(const char* errorMessage) :
	runtime_error(errorMessage)
{
}

CommandException::CommandException(const std::string& errorMessage) :
	runtime_error(errorMessage)
{
}
