#include "SaveCommand.h"
#include "Graph IO\Graph Saver\GraphSaver.h"
#include "Graph Collection\GraphCollection.h"
#include "Command\Command Registrator\CommandRegistrator.h"

static CommandRegistrator<SaveCommand> registrator("save", "Saves all graphs");

void SaveCommand::parseArguments(args::Subparser& parser)
{
	Command::parseArguments(parser);
}

void SaveCommand::doExecute()
{
	GraphIO::GraphSaver saver;
	const GraphCollection& graphs = getGraphs();
	GraphCollection::ConstIterator iterator =
		graphs.getConstIterator();

	forEach(iterator, [&](const Graph& g)
	{
		saver.save(g);
	});
}
