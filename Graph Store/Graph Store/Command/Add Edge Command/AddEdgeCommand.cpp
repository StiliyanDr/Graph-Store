#include "AddEdgeCommand.h"
#include "Command/Command Registrator/CommandRegistrator.h"

static CommandRegistrator<AddEdgeCommand> registrator("add-edge",
													  "Adds an edge between two vertices with a specified weight");

void AddEdgeCommand::parseArguments(args::Subparser& parser)
{
	PositionalString startID(parser, "start vertex id", "The id of the start vertex");
	PositionalString endID(parser, "end vertex id", "The id of the end vertex");
	PositionalWeight weight(parser, "weight", "The weight of the edge");

	parser.Parse();

	setStartVertexID(startID);
	setEndVertexID(endID);
	setWeight(weight);
}

void AddEdgeCommand::setStartVertexID(PositionalString& id)
{
	startVertexID = getValueOf(id);
}

void AddEdgeCommand::setEndVertexID(PositionalString& id)
{
	endVertexID = getValueOf(id);
}

void AddEdgeCommand::setWeight(PositionalWeight& w)
{
	weight = w.Matched() ? args::get(w) : DEFAULT_EDGE_WEIGHT;
}

void AddEdgeCommand::doExecute()
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& start = usedGraph.getVertexWithID(startVertexID);
	Graph::Vertex& end = usedGraph.getVertexWithID(endVertexID);

	usedGraph.addEdge(start, end, weight);
}
