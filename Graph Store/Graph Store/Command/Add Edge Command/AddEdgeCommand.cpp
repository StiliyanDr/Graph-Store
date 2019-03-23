#include "AddEdgeCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<AddEdgeCommand> registrator("add-edge",
													  "Adds an edge between two vertices with a specified weight");

void AddEdgeCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	addEdge(startVertexID, endVertexID, weight);
}

void AddEdgeCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> startID(parser, "start vertex id", "The identifier of the start vertex");
	args::Positional<String, StringReader> endID(parser, "end vertex id", "The identifier of the end vertex");
	args::Positional<Graph::Edge::Weight> weight(parser, "weight", "The weight of the edge");

	parser.Parse();

	setStartVertexID(startID);
	setEndVertexID(endID);
	setWeight(weight);
}

void AddEdgeCommand::setStartVertexID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		startVertexID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void AddEdgeCommand::setEndVertexID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		endVertexID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void AddEdgeCommand::setWeight(args::Positional<Graph::Edge::Weight>& weight)
{
	this->weight = weight.Matched() ? args::get(weight) : DEFAULT_EDGE_WEIGHT;
}

void AddEdgeCommand::addEdge(const String& startVertexID,
							 const String& endVertexID,
							 Graph::Edge::Weight weight)
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& start = usedGraph.getVertexWithID(startVertexID);
	Graph::Vertex& end = usedGraph.getVertexWithID(endVertexID);

	usedGraph.addEdge(start, end, weight);
}
