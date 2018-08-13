#include "LoadCommand.h"
#include "../../Logger/Logger.h"
#include "../../Directory Loader/DirectoryLoader.h"
#include "../Command Registrator/CommandRegistrator.h"

static CommandRegistrator<LoadCommand> registrator("LOAD", "Loads files in a specified directory");

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
		throw std::runtime_error("Missing argument [path]!");
	}
}

void LoadCommand::loadGraphs(const String& path)
{
	DirectoryLoader loader;

	loader.loadApplyingFunctionToEachGraph(path, tryToAddGraph);
}

void LoadCommand::tryToAddGraph(std::unique_ptr<Graph> graph)
{
	GraphCollection& graphs = getGraphs();

	try
	{
		graphs.add(std::move(graph));
	}
	catch (std::runtime_error& e)
	{
		Logger::logError(e);
	}
}
