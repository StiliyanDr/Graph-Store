#include "UseGraphCommand.h"
#include "../Command Registrator/CommandRegistrator.h"

static CommandRegistrator<UseGraphCommand> registrator("USE-GRAPH", "Sets a specified graph as the used one");

void UseGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	useGraph(graphID);
}

void UseGraphCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> graphID(parser, "graph id", "The identifier of the graph to use");
	parser.Parse();
	setGraphID(graphID);
}

void UseGraphCommand::setGraphID(args::Positional<String, StringReader>& graphID)
{
	if (graphID.Matched())
	{
		this->graphID = args::get(graphID);
	}
	else
	{
		throw std::runtime_error("Missing argument: [" + graphID.Name() + "]!");
	}
}

void UseGraphCommand::useGraph(const String& id)
{
	GraphCollection& graphs = getGraphs();
	Graph& graph = graphs.getGraphWithID(id);
	setUsedGraph(graph);
}
