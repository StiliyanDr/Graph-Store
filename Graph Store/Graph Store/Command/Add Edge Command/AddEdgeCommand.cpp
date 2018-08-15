#include "AddEdgeCommand.h"
#include "../Command Registrator/CommandRegistrator.h"

static CommandRegistrator<AddEdgeCommand> registrator("ADD-EDGE",
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
	args::Positional<unsigned> weight(parser, "weight", "The weight of the edge");

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
		throw std::runtime_error("Missing argument: [" + id.Name() + "]!");
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
		throw std::runtime_error("Missing argument: [" + id.Name() + "]!");
	}
}

void AddEdgeCommand::setWeight(args::Positional<unsigned>& weight)
{
	this->weight = weight.Matched() ? args::get(weight) : DEFAULT_EDGE_WEIGHT;
}

void AddEdgeCommand::addEdge(const String& startVertexID,
							 const String& endVertexID,
							 unsigned weight)
{
	Graph& usedGraph = getUsedGraph();
	Vertex& start = usedGraph.getVertexWithIdentifier(startVertexID);
	Vertex& end = usedGraph.getVertexWithIdentifier(endVertexID);

	usedGraph.addEdgeBetweenWithWeight(start, end, weight);
}
