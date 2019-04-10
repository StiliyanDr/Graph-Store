#include "Command.h"
#include "Command\Exceptions\MissingArgumentException.h"
#include "String\String.h"

void Command::StringReader::operator()(const std::string& name,
									   const std::string& value,
									   String& destination) const
{
	destination = value.c_str();
}

void Command::execute(args::Subparser& parser)
{
	parseArguments(parser);
	doExecute();
}

void Command::parseArguments(args::Subparser& parser)
{
	parser.Parse();
}

String Command::getValueOf(PositionalString& argument)
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
