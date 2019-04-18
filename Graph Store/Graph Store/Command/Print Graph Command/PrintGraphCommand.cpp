#include "PrintGraphCommand.h"
#include "Command/Command Registrator/CommandRegistrator.h"
#include "Graph Collection/GraphCollection.h"
#include "Graph IO/GraphIOConstants.h"
#include <iostream>

static CommandRegistrator<PrintGraphCommand> registrator("print-graph",
	                                                     "Prints a specified graph");

void PrintGraphCommand::parseArguments(args::Subparser& parser)
{
	PositionalString id(parser, "graph id", "The id of the graph to print");
	parser.Parse();
	setGraphID(id);
}

void PrintGraphCommand::setGraphID(PositionalString& id)
{
	graphID = id.Matched() ? args::get(id) : getUsedGraph().getID();
}

void PrintGraphCommand::doExecute()
{
	GraphCollection& graphs = getGraphs();
	Graph& graph = graphs[graphID];

	Graph::VerticesConstIterator iterator =
		graph.getConstIteratorOfVertices();

	forEach(*iterator, [&](const Graph::Vertex& v)
	{
		printEdgesLeaving(v, graph);
	});
}

void PrintGraphCommand::printEdgesLeaving(const Graph::Vertex& v,
	                                      const Graph& g)
{
	std::cout << v.getID() << ": ";

	Graph::OutgoingEdgesConstIterator iterator =
		g.getConstIteratorOfEdgesLeaving(v);

	forEach(*iterator, [&](const Graph::OutgoingEdge& e)
	{
		printEdge(e);
	});

	std::cout << '\n';
}

void PrintGraphCommand::printEdge(const Graph::OutgoingEdge& e)
{
	using namespace GraphIO;

	const Graph::Vertex& end = e.getEnd();

	std::cout << EDGE_START << e.getWeight()
		      << EDGE_ATTRIBUTE_SEPARATOR << ' '
		      << end.getID() << EDGE_END << ", ";
}
