#include "RemoveVertexCommand.h"
#include "Command/Command Registrator/CommandRegistrator.h"
#include "Graph/Abstract class/Graph.h"

static CommandRegistrator<RemoveVertexCommand> registrator("remove-vertex",
														   "Removes a vertex with a specified id");

void RemoveVertexCommand::parseArguments(args::Subparser& parser)
{
	PositionalString id(parser, "vertex id", "The id of the vertex to remove");
	parser.Parse();
	vertexID = getValueOf(id);
}

void RemoveVertexCommand::doExecute()
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& v = usedGraph.getVertexWithID(vertexID);

	usedGraph.removeVertex(v);
}
