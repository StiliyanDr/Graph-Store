#include "SearchCommand.h"
#include "Shortest Path Algorithms/Store/ShortestPathAlgorithmsStore.h"
#include "Command/Command Registrator/CommandRegistrator.h"

static CommandRegistrator<SearchCommand> registrator("search",
	                                                 "Finds a shortest path between two vertices");

void SearchCommand::parseArguments(args::Subparser& parser)
{
	PositionalString sourceID(parser, "source id", "The id of the source vertex");
	PositionalString targetID(parser, "target id", "The id of the target vertex");
	PositionalString algorithmID(parser, "algorithm id", "The algorithm to use");

	parser.Parse();

	this->sourceID = getValueOf(sourceID);
	this->targetID = getValueOf(targetID);
	this->algorithmID = getValueOf(algorithmID);
}

void SearchCommand::doExecute()
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& source = usedGraph.getVertexWithID(sourceID);
	Graph::Vertex& target = usedGraph.getVertexWithID(targetID);

	ShortestPathAlgorithm& algorithm =
		ShortestPathAlgorithmsStore::instance().search(algorithmID);

	ShortestPathAlgorithm::Path p =
		algorithm.findShortestPath(usedGraph, source, target);

	std::cout << p << '\n';
}
