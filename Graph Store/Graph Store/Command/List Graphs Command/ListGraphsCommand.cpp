#include "ListGraphsCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include <iostream>

static CommandRegistrator<ListGraphsCommand> registrator("LIST-GRAPHS",
														 "Lists the identifiers of all graphs");

void ListGraphsCommand::execute(args::Subparser& parser)
{
	parser.Parse();
	listIdentifiersOfGraphs();
}

void ListGraphsCommand::listIdentifiersOfGraphs()
{
	GraphCollection& graphs = getGraphs();

	std::unique_ptr<Iterator<Graph*>> iterator = graphs.getIterator();

	forEach(*iterator, [&](const Graph* graph)
	{
		std::cout << graph->getID() << '\n';
	});
}
