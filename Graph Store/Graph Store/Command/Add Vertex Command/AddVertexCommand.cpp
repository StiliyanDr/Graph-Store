#include "AddVertexCommand.h"
#include "Command\Command Registrator\CommandRegistrator.h"

static CommandRegistrator<AddVertexCommand> registrator("add-vertex",
	                                                    "Adds a vertex with a specified id");

void AddVertexCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser,
		                                      "vertex id",
		                                      "The id of the new vertex");
	parser.Parse();
	vertexID = getValueOf(id);
}

void AddVertexCommand::doExecute()
{
	Graph& usedGraph = getUsedGraph();
	
	usedGraph.addVertex(vertexID);
}
