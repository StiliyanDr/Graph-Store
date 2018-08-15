#include "RemoveGraphCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<RemoveGraphCommand> registrator("REMOVE-GRAPH", "Removes a specified graph");

void RemoveGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	removeGraph(graphID);
}

void RemoveGraphCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser, "graph id", "The identifier of the graph to remove");
	parser.Parse();
	setGraphID(id);
}

void RemoveGraphCommand::setGraphID(args::Positional<String, StringReader>& id)
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

void RemoveGraphCommand::removeGraph(const String& id)
{
	GraphCollection& graphs = getGraphs();
	std::unique_ptr<Graph> graph = graphs.remove(id);

	if (isUsedGraph(*graph))
	{
		useNoGraph();
	}
}
