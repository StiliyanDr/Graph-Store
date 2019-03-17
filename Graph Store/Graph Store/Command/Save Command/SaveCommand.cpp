#include "SaveCommand.h"
#include "Graph IO\Graph Saver\GraphSaver.h"
#include "Command\Command Registrator\CommandRegistrator.h"

static CommandRegistrator<SaveCommand> registrator("SAVE", "Saves all graphs");

void SaveCommand::execute(args::Subparser& parser)
{
	parser.Parse();
	saveGraphs();
}

void SaveCommand::saveGraphs()
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
