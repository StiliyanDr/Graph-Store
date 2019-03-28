#include "Command.h"
#include "Runtime Error/RuntimeError.h"
#include "Graph Collection/GraphCollection.h"
#include "Command\Exceptions\Missing Argument Exception\MissingArgumentException.h"

void Command::StringReader::operator()(const std::string& name,
									   const std::string& value,
									   String& destination) const
{
	destination = value.c_str();
}

Graph* Command::usedGraph = nullptr;
GraphCollection* Command::graphs = nullptr;

GraphCollection& Command::getGraphs()
{
	assert(graphs != nullptr);

	return *graphs;
}

bool Command::isUsedGraph(const String& id)
{
	return usedGraph != nullptr
		   && usedGraph->getID() == id;
}

Graph& Command::getUsedGraph()
{
	if (usedGraph != nullptr)
	{
		return *usedGraph;
	}
	else
	{
		throw RuntimeError("No graph is currently used!"_s);
	}
}

void Command::useNoGraph()
{
	usedGraph = nullptr;
}

void Command::setUsedGraph(Graph& graph)
{
	usedGraph = &graph;
}

void Command::setGraphs(GraphCollection& collection)
{
	graphs = &collection;
}

String
Command::getValueOf(args::Positional<String, StringReader>& argument)
{
	if (argument.Matched())
	{
		return args::get(argument);
	}
	else
	{
		throw MissingArgumentException(argument.Name());
	}
}
