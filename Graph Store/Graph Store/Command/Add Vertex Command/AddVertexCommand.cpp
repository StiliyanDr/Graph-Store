#include "AddVertexCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<AddVertexCommand> registrator("add-vertex", "Adds a vertex with a specified identifier");

void AddVertexCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	addVertex(vertexID);
}

void AddVertexCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser, "vertex id", "The identifier of the new vertex");
	parser.Parse();
	setVertexID(id);
}

void AddVertexCommand::setVertexID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		vertexID = args::get(id);
	}
	else
	{
		throw MissingArgumentException(id.Name());
	}
}

void AddVertexCommand::addVertex(const String& id)
{
	Graph& usedGraph = getUsedGraph();
	
	usedGraph.addVertex(id);
}
