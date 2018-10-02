#include "IterativeDeepeningDFS.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<IterativeDeepeningDFS> registrator("dfs-shortest");

IterativeDeepeningDFS::IterativeDeepeningDFS(const String& id) :
	SearchBasedShortestPathAlgorithm(id)
{
}

void IterativeDeepeningDFS::initialise(const Graph& graph,
									   const Vertex& source,
									   const Vertex& target)
{
	decorateVerticesOf(graph);
	initialiseSource(getDecoratedVersionOf(source));
	setGraph(graph);
	setTarget(target);
	foundAShortestPath = false;
}

void IterativeDeepeningDFS::execute(const Graph& graph,
									const Vertex& source,
									const Vertex& target)
{
	unsigned maxLengthOfShortestPath = graph.getVerticesCount() - 1;
	unsigned depth = 0;

	while (!foundAShortestPath && depth <= maxLengthOfShortestPath)
	{
		depthLimitedSearch(getDecoratedVersionOf(source), depth);
		++depth;
	}
}

void IterativeDeepeningDFS::depthLimitedSearch(MarkableDecoratedVertex& vertex,
											   unsigned depth)
{
	vertex.isVisited = true;

	if (depth == 0)
	{
		checkIfTarget(vertex);
	}
	else
	{
		expandSearch(vertex, depth - 1);
	}

	vertex.isVisited = false;
}

void IterativeDeepeningDFS::expandSearch(const MarkableDecoratedVertex& predecessor,
										 unsigned depth)
{
	Graph::EdgesConstIterator iterator =
		graph->getConstIteratorOfEdgesLeaving(predecessor.originalVertex);

	while (!foundAShortestPath && *iterator)
	{
		MarkableDecoratedVertex& successor =
			getDecoratedVersionOf((*iterator)->getVertex());

		if (!successor.isVisited)
		{
			visitVertex(successor, predecessor);
			depthLimitedSearch(successor, depth);
		}

		++(*iterator);
	}
}

void IterativeDeepeningDFS::setGraph(const Graph& graph)
{
	this->graph = &graph;
}
