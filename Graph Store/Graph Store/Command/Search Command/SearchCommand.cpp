#include "SearchCommand.h"
#include "../../Shortest Path Algorithms/Store/ShortestPathAlgorithmsStore.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../../Shortest Path Algorithms/Path Printing/PathPrinting.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<SearchCommand> registrator("SEARCH", "Finds a shortest path between two vertices");

void SearchCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	PathEnds ends = findShortestPath(sourceID, targetID, algorithmID);
	printPathAndItsLength(ends.source, ends.target);
}

void SearchCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> sourceID(parser, "source id", "The identifier of the source vertex");
	args::Positional<String, StringReader> targetID(parser, "target id", "The identifier of the target vertex");
	args::Positional<String, StringReader> algorithmID(parser, "algorithm id", "The algorithm to use");

	parser.Parse();

	setSourceID(sourceID);
	setTargetID(targetID);
	setAlgorithmID(algorithmID);
}

void SearchCommand::setSourceID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		sourceID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void SearchCommand::setTargetID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		targetID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void SearchCommand::setAlgorithmID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		algorithmID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

SearchCommand::PathEnds SearchCommand::findShortestPath(const String& sourceID,
														const String& targetID,
														const String& algorithmID)
{
	Graph& usedGraph = getUsedGraph();
	Vertex& source = usedGraph.getVertexWithIdentifier(sourceID);
	Vertex& target = usedGraph.getVertexWithIdentifier(targetID);

	ShortestPathAlgorithm& algorithm =
		ShortestPathAlgorithmsStore::instance().searchForAlgorithm(algorithmID);

	algorithm.findShortestPath(usedGraph, source, target);

	return PathEnds(source, target);
}
