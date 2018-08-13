#include "Command.h"

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
		throw std::runtime_error("No graph is currently used!");
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
