#include "Command.h"
#include "../../Runtime Error/RuntimeError.h"

Graph* Command::usedGraph = nullptr;
GraphCollection Command::graphs;

GraphCollection& Command::getGraphs()
{
	return graphs;
}

bool Command::isUsedGraph(const Graph& graph)
{
	return usedGraph == &graph;
}

Graph& Command::getUsedGraph()
{
	if (usedGraph != nullptr)
	{
		return *usedGraph;
	}
	else
	{
		throw RuntimeError(String("No graph is currently used!"));
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
