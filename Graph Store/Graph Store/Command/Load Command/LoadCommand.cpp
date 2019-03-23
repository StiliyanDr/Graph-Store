#include "LoadCommand.h"
#include "Logger/Logger.h"
#include "Graph IO/Directory Loader/DirectoryLoader.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<LoadCommand> registrator("load", "Loads files in a specified directory");

void LoadCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	loadGraphs(path);
}

void LoadCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> path(parser, "path", "Path to the directory to load");
	parser.Parse();
	setPath(path);
}

void LoadCommand::setPath(args::Positional<String, StringReader>& path)
{
	if (path.Matched())
	{
		this->path = args::get(path);
	}
	else
	{
		throw MissingArgumentException(path.Name());
	}
}

void LoadCommand::loadGraphs(const String& path)
{
	GraphIO::DirectoryLoader loader;

	setGraphs(loader.load(path));
}
