#include "PrintGraphCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"
#include <iostream>

static CommandRegistrator<PrintGraphCommand> registrator("PRINT-GRAPH", "Prints a specified graph");

void PrintGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	printGraph(graphID);
}

void PrintGraphCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser, "graph id", "The identifier of the graph to print");
	parser.Parse();
	setGraphID(id);
}

void PrintGraphCommand::setGraphID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		graphID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void PrintGraphCommand::printGraph(const String& id)
{
	GraphCollection& graphs = getGraphs();
	Graph& graph = graphs.getGraphWithID(id);

	Graph::VerticesConstIterator iterator =
		graph.getConstIteratorOfVertices();

	forEach(*iterator, [&](const Graph::Vertex* v)
	{
		printEdgesLeaving(*v, graph);
	});
}

void PrintGraphCommand::printEdgesLeaving(const Graph::Vertex& v, const Graph& g)
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
	const Graph::Vertex& end = e.getVertex();

	std::cout << "(" << e.getWeight() << ", "
			  << end.getID() << "), ";
}
