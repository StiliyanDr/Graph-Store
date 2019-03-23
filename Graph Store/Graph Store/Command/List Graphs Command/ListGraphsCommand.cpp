#include "ListGraphsCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include <iostream>

static CommandRegistrator<ListGraphsCommand> registrator("list-graphs",
														 "Lists the identifiers of all graphs");

void ListGraphsCommand::execute(args::Subparser& parser)
{
	parser.Parse();
	listIdentifiersOfGraphs();
}

void ListGraphsCommand::listIdentifiersOfGraphs()
{
	GraphCollection& graphs = getGraphs();

	GraphCollection::ConstIterator iterator =
		graphs.getConstIterator();

	forEach(iterator, [](const Graph& graph)
	{
		std::cout << graph.getID() << '\n';
	});
}
