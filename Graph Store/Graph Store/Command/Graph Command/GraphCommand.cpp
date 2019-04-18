#include "GraphCommand.h"
#include "Runtime Error/RuntimeError.h"
#include  "Graph Collection/GraphCollection.h"
#include <assert.h>

Graph* GraphCommand::usedGraph = nullptr;
GraphCollection* GraphCommand::graphs = nullptr;

GraphCollection& GraphCommand::getGraphs()
{
	assert(graphs != nullptr);

	return *graphs;
}

Graph& GraphCommand::getUsedGraph()
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

void GraphCommand::setUsedGraph(Graph& g)
{
	usedGraph = &g;
}

void GraphCommand::useNoGraph()
{
	usedGraph = nullptr;
}

bool GraphCommand::isUsedGraph(const String& id)
{
	return usedGraph != nullptr
		   && usedGraph->getID() == id;
}

void GraphCommand::setGraphs(GraphCollection& collection)
{
	graphs = &collection;
}
