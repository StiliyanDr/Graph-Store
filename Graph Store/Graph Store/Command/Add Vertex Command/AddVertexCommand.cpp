#include "AddVertexCommand.h"
#include "Command\Command Registrator\CommandRegistrator.h"
#include "Graph\Abstract class\Graph.h"

static CommandRegistrator<AddVertexCommand> registrator("add-vertex",
	                                                    "Adds a vertex with a specified id");

void AddVertexCommand::parseArguments(args::Subparser& parser)
{
	PositionalString id(parser, "vertex id", "The id of the new vertex");
	parser.Parse();
	vertexID = getValueOf(id);
}

void AddVertexCommand::doExecute()
{
	Graph& usedGraph = getUsedGraph();
	
	usedGraph.addVertex(vertexID);
}
