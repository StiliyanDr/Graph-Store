#include "RemoveGraphCommand.h"
#include "Command\Command Registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveGraphCommand> registrator("remove-graph",
	                                                      "Removes a specified graph");

void RemoveGraphCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser,
		                                      "graph id",
		                                      "The id of the graph to remove");
	parser.Parse();
	graphID = getValueOf(id);
}

void RemoveGraphCommand::doExecute()
{
	if (isUsedGraph(graphID))
	{
		useNoGraph();
	}

	GraphCollection& graphs = getGraphs();
	graphs.remove(graphID);
}
