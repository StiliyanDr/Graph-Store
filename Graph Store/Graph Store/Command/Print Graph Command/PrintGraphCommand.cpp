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

	std::unique_ptr<Iterator<Vertex*>> iterator = graph.getIteratorOfVertices();

	forEach(*iterator, [&](Vertex* vertex)
	{
		printEdgesLeaving(*vertex, graph);
	});
}

void PrintGraphCommand::printEdgesLeaving(Vertex& vertex, Graph& graph)
{
	std::cout << vertex.getID() << ": ";

	std::unique_ptr<Iterator<Edge>> iterator =
		graph.getIteratorOfEdgesLeaving(vertex);

	forEach(*iterator, [&](Edge& e)
	{
		printEdge(e);
	});

	std::cout << '\n';
}

void PrintGraphCommand::printEdge(Edge& edge)
{
	Vertex& endOfEdge = edge.getVertex();

	std::cout << "(" << edge.getWeight() << ", "
			  << endOfEdge.getID() << "), ";
}
