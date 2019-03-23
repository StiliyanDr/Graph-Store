#include "RemoveVertexCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<RemoveVertexCommand> registrator("remove-vertex",
														   "Removes a vertex with a specified identifier");

void RemoveVertexCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	removeVertex(vertexID);
}

void RemoveVertexCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser, "vertex id", "The identifier of the vertex to remove");
	parser.Parse();
	setVertexID(id);
}

void RemoveVertexCommand::setVertexID(args::Positional<String, StringReader>& id)
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

void RemoveVertexCommand::removeVertex(const String& id)
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& vertex = usedGraph.getVertexWithID(id);

	usedGraph.removeVertex(vertex);
}
