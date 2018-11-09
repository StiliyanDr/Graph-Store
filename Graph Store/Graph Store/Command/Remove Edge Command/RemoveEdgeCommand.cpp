#include "RemoveEdgeCommand.h"
#include "../Command Registrator/CommandRegistrator.h"
#include "../Exceptions/Missing Argument Exception/MissingArgumentException.h"

static CommandRegistrator<RemoveEdgeCommand> registrator("REMOVE-EDGE",
														 "Removes the edge between two specified vertices");

void RemoveEdgeCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	removeEdge(startVertexID, endVertexID);
}

void RemoveEdgeCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> startID(parser, "start vertex id", "The identifier of the start vertex");
	args::Positional<String, StringReader> endID(parser, "end vertex id", "The identifier of the end vertex");

	parser.Parse();

	setStartVertexID(startID);
	setEndVertexID(endID);
}

void RemoveEdgeCommand::setStartVertexID(args::Positional<String, StringReader>& id)
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

void RemoveEdgeCommand::setEndVertexID(args::Positional<String, StringReader>& id)
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

void RemoveEdgeCommand::removeEdge(const String& startVertexID, const String& endVertexID)
{
	Graph& usedGraph = getUsedGraph();
	Graph::Vertex& start = usedGraph.getVertexWithID(startVertexID);
	Graph::Vertex& end = usedGraph.getVertexWithID(endVertexID);

	usedGraph.removeEdge(start, end);
}
