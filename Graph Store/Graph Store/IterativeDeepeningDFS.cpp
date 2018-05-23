#include "stdafx.h"
#include "IterativeDeepeningDFS.h"
#include "Graph.h"
#include "Vertex.h"

IterativeDeepeningDFS IterativeDeepeningDFS::theOnlyInstance("dfs-shortest");

IterativeDeepeningDFS::IterativeDeepeningDFS(const char* identifier) :
	ShortestPathAlgorithm(identifier)
{
}

void IterativeDeepeningDFS::findShortestPath(Graph& graph, Vertex& source, Vertex& destination)
{
	initialiseAlgorithm(graph, source, destination);

	unsigned maxLengthOfShortestPath = graph.getVerticesCount() - 1;
	unsigned depth = 0;

	while (!hasFoundAShortestPath && depth <= maxLengthOfShortestPath)
	{
		depthLimitedSearch(source, depth);
		++depth;
	}
}

void IterativeDeepeningDFS::initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& destination)
{
	initialiseVerticesOf(graph);
	initialiseSource(source);
	setGraph(graph);
	setDestination(destination);
	hasFoundAShortestPath = false;
}

void IterativeDeepeningDFS::initialiseVertex(Vertex& vertex) const
{
	ShortestPathAlgorithm::initialiseVertex(vertex);
}

void IterativeDeepeningDFS::initialiseSource(Vertex& source) const
{
	source.setDistance(0);
}

void IterativeDeepeningDFS::depthLimitedSearch(Vertex& vertex, unsigned depth)
{
	vertex.markAsVisited();

	if (depth == 0) 
	{
		checkIfDestination(vertex);
	}
	else
	{
		expandSearch(vertex, depth - 1);
	}

	vertex.markAsNotVisited();
}

void IterativeDeepeningDFS::checkIfDestination(const Vertex& vertex)
{
	if (&vertex == destination)
	{
		assert(!hasFoundAShortestPath);

		hasFoundAShortestPath = true;
	}
}

void IterativeDeepeningDFS::expandSearch(Vertex& vertex, unsigned depth)
{
	std::unique_ptr<Iterator<Edge>> edgesIterator = graph->getIteratorOfEdgesStartingFrom(vertex);

	while (!hasFoundAShortestPath && edgesIterator->isValid())
	{
		Vertex& endOfEdgeVertex = edgesIterator->getCurrentItem().getVertex();

		if (!endOfEdgeVertex.isMarkedAsVisited())
		{
			visitVertex(endOfEdgeVertex, vertex);
			depthLimitedSearch(endOfEdgeVertex, depth);
		}

		edgesIterator->advance();
	}
}

void IterativeDeepeningDFS::visitVertex(Vertex& vertex, Vertex& predecessor) const
{
	vertex.setParent(&predecessor);
	vertex.setDistance(predecessor.getDistance() + 1);
}

void IterativeDeepeningDFS::setDestination(const Vertex& destination)
{
	this->destination = &destination;
}

void IterativeDeepeningDFS::setGraph(Graph& graph)
{
	this->graph = &graph;
}
