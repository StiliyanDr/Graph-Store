#include "RemoveEdgeCommand.h"
#include "Command\Command Registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveEdgeCommand> registrator("remove-edge",
														 "Removes the edge between two specified vertices");

void RemoveEdgeCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> startID(parser,
		                                           "start vertex id",
		                                           "The id of the start vertex");
	args::Positional<String, StringReader> endID(parser,
		                                         "end vertex id",
		                                         "The id of the end vertex");
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
