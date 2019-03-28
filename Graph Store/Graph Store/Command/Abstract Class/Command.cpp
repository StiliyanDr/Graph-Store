#include "Command.h"
#include "Command\Exceptions\Missing Argument Exception\MissingArgumentException.h"
#include "String\String.h"

void Command::StringReader::operator()(const std::string& name,
									   const std::string& value,
									   String& destination) const
{
	destination = value.c_str();
}

String
Command::getValueOf(args::Positional<String, StringReader>& argument)
{
	if (argument.Matched())
	{
		return args::get(argument);
	}
	else
	{
		throw MissingArgumentException(argument.Name());
	}
}
