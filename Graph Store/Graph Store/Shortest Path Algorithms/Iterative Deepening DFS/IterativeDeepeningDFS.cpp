#include "IterativeDeepeningDFS.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<IterativeDeepeningDFS> registrator("dfs-shortest");

IterativeDeepeningDFS::IterativeDeepeningDFS(const char* id) :
	ShortestPathAlgorithm(id)
{
}

void IterativeDeepeningDFS::findShortestPath(Graph& graph, Vertex& source, Vertex& target)
{
	initialiseAlgorithm(graph, source, target);

	unsigned maxLengthOfShortestPath = graph.getVerticesCount() - 1;
	unsigned depth = 0;

	while (!hasFoundAShortestPath && depth <= maxLengthOfShortestPath)
	{
		depthLimitedSearch(source, depth);
		++depth;
	}
}

void IterativeDeepeningDFS::initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& target)
{
	initialiseVerticesOf(graph);
	initialiseSource(source);
	setGraph(graph);
	setTarget(target);
	hasFoundAShortestPath = false;
}

void IterativeDeepeningDFS::initialiseVertex(Vertex& vertex) const
{
	ShortestPathAlgorithm::initialiseVertex(vertex);
}

void IterativeDeepeningDFS::depthLimitedSearch(Vertex& vertex, unsigned depth)
{
	vertex.markAsVisited();

	if (depth == 0) 
	{
		checkIfTarget(vertex);
	}
	else
	{
		expandSearch(vertex, depth - 1);
	}

	vertex.markAsNotVisited();
}

void IterativeDeepeningDFS::checkIfTarget(const Vertex& vertex)
{
	if (vertex == *target)
	{
		assert(!hasFoundAShortestPath);

		hasFoundAShortestPath = true;
	}
}

void IterativeDeepeningDFS::expandSearch(Vertex& vertex, unsigned depth)
{
	std::unique_ptr<Iterator<Edge>> iterator = graph->getIteratorOfEdgesStartingFrom(vertex);

	while (!hasFoundAShortestPath && *iterator)
	{
		Vertex& endOfEdgeVertex = (*iterator)->getVertex();

		if (!endOfEdgeVertex.isMarkedAsVisited())
		{
			visitVertex(endOfEdgeVertex, vertex);
			depthLimitedSearch(endOfEdgeVertex, depth);
		}

		++(*iterator);
	}
}

void IterativeDeepeningDFS::visitVertex(Vertex& vertex, Vertex& predecessor) const
{
	vertex.setParent(&predecessor);
	vertex.setDistance(predecessor.getDistance() + 1);
}

void IterativeDeepeningDFS::setTarget(const Vertex& target)
{
	this->target = &target;
}

void IterativeDeepeningDFS::setGraph(Graph& graph)
{
	this->graph = &graph;
}
