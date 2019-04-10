#include "UseGraphCommand.h"
#include "Command/Command Registrator/CommandRegistrator.h"

static CommandRegistrator<UseGraphCommand> registrator("use-graph",
	                                                   "Sets a specified graph as the used one");

void UseGraphCommand::parseArguments(args::Subparser& parser)
{
	PositionalString id(parser, "graph id", "The id of the graph to use");
	parser.Parse();
	graphID = getValueOf(id);
}

void UseGraphCommand::doExecute()
{
	GraphCollection& graphs = getGraphs();
	setUsedGraph(graphs[graphID]);
}
