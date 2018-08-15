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
	args::Positional<String, StringReader> graphID(parser, "graph id", "The identifier of the graph to print");
	parser.Parse();
	setGraphID(graphID);
}

void PrintGraphCommand::setGraphID(args::Positional<String, StringReader>& graphID)
{
	if (graphID.Matched())
	{
		this->graphID = args::get(graphID);
	}
	else
	{
		throw MissingArgumentException(graphID.Name());
	}
}

void PrintGraphCommand::printGraph(const String& graphID)
{
	GraphCollection& graphs = getGraphs();
	Graph& graph = graphs.getGraphWithID(graphID);

	std::unique_ptr<Iterator<Vertex*>> iterator = graph.getIteratorOfVertices();
	Vertex* vertex;

	while (iterator->isValid())
	{
		vertex = iterator->getCurrentItem();

		printEdgesLeaving(*vertex, graph);

		iterator->advance();
	}
}

void PrintGraphCommand::printEdgesLeaving(Vertex& vertex, Graph& graph)
{
	std::cout << vertex.getIdentifier() << ": ";

	std::unique_ptr<Iterator<Edge>> iterator =
		graph.getIteratorOfEdgesStartingFrom(vertex);

	while (iterator->isValid())
	{
		printEdge(iterator->getCurrentItem());

		iterator->advance();
	}

	std::cout << '\n';
}

void PrintGraphCommand::printEdge(Edge& edge)
{
	Vertex& endOfEdge = edge.getVertex();

	std::cout << "(" << edge.getWeight() << ", "
			  << endOfEdge.getIdentifier() << "), ";
}
