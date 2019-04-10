#include "ListGraphsCommand.h"
#include "Command\Command Registrator\CommandRegistrator.h"
#include "Graph Collection\GraphCollection.h"
#include <iostream>

static CommandRegistrator<ListGraphsCommand> registrator("list-graphs",
														 "Lists the ids of all graphs");

void ListGraphsCommand::parseArguments(args::Subparser& parser)
{
	Command::parseArguments(parser);
}

void ListGraphsCommand::doExecute()
{
	GraphCollection& graphs = getGraphs();

	GraphCollection::ConstIterator iterator =
		graphs.getConstIterator();

	forEach(iterator, [](const Graph& g)
	{
		std::cout << g.getID() << '\n';
	});
}
