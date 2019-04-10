#include "RemoveEdgeCommand.h"
#include "Command\Command Registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveEdgeCommand> registrator("remove-edge",
														 "Removes the edge between two specified vertices");

void RemoveEdgeCommand::parseArguments(args::Subparser& parser)
{
	PositionalString startID(parser, "start vertex id", "The id of the start vertex");
	PositionalString endID(parser, "end vertex id", "The id of the end vertex");

	parser.Parse();

	this->startID = getValueOf(startID);
	this->endID = getValueOf(endID);
}

void RemoveEdgeCommand::doExecute()
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& start = usedGraph.getVertexWithID(startID);
	Graph::Vertex& end = usedGraph.getVertexWithID(endID);

	usedGraph.removeEdge(start, end);
}
