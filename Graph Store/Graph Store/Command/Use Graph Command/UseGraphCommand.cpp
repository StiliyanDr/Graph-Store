#include "UseGraphCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<UseGraphCommand> registrator("use-graph", "Sets a specified graph as the used one");

void UseGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	useGraph(graphID);
}

void UseGraphCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser, "graph id", "The identifier of the graph to use");
	parser.Parse();
	setGraphID(id);
}

void UseGraphCommand::setGraphID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		graphID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void UseGraphCommand::useGraph(const String& id)
{
	GraphCollection& graphs = getGraphs();
	Graph& graph = graphs[id];
	setUsedGraph(graph);
}
