#include "Command.h"
#include "Runtime Error/RuntimeError.h"
#include "Graph Collection/GraphCollection.h"

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
