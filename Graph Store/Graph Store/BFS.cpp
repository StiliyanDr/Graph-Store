#include "stdafx.h"
#include "BFS.h"

BFS BFS::theOnlyInstance("bfs");

BFS::BFS(const char* algorithmIdentifier) :
	ShortestPathAlgorithm(algorithmIdentifier)
{
}

void BFS::findShortestPath(Graph& graph, Vertex& source, Vertex& destination)
{
	initialiseAlgorithm(graph, source, destination);

	Vertex* vertex;
	std::unique_ptr<Iterator<Edge>> edgesIterator;

	while (!(hasFoundAShortestPath || theFrontierIsEmpty()))
	{
		vertex = getNextVertexFromFrontier();
		edgesIterator = graph.getIteratorOfEdgesStartingFrom(*vertex);

		while (edgesIterator->isValid())
		{
			exploreEdge(*vertex, edgesIterator->getCurrentItem().getVertex());
			edgesIterator->advance();
		}
	}

	cleanUpAlgorithmState();
}

void BFS::initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& destination)
{
	assert(theFrontierIsEmpty());

	if (&source != &destination)
	{
		hasFoundAShortestPath = false;
		setDestination(destination);
		initialiseVerticesOf(graph);
		initialiseSource(source);
		addVertexToFrontier(source);
	}
	else
	{
		hasFoundAShortestPath = true;
		buildTrivialPathStartingFrom(source);
	}
}

void BFS::exploreEdge(Vertex& predecessor, Vertex& successor)
{
	if (successor.isMarkedAsVisited())
	{
		return;
	}

	visitVertex(successor, predecessor);
	addVertexToFrontier(successor);
	checkIfDestination(successor);
}

void BFS::visitVertex(Vertex& vertex, Vertex& predecessor) const
{
	assert(!vertex.isMarkedAsVisited());

	vertex.markAsVisited();
	vertex.setParent(&predecessor);
	vertex.setDistance(predecessor.getDistance() + 1);
}

void BFS::initialiseVertex(Vertex& vertex) const
{
	ShortestPathAlgorithm::initialiseVertex(vertex);
}

void BFS::initialiseSource(Vertex& source) const
{
	source.markAsVisited();
	source.setDistance(0);
}

void BFS::buildTrivialPathStartingFrom(Vertex& vertex)
{
	vertex.setParent(nullptr);
	vertex.setDistance(0);
}

bool BFS::theFrontierIsEmpty() const
{
	return queue.isEmpty();
}

Vertex* BFS::getNextVertexFromFrontier()
{
	return queue.dequeue();
}

void BFS::addVertexToFrontier(Vertex& vertex)
{
	queue.enqueue(&vertex);
}

void BFS::cleanUpAlgorithmState()
{
	queue.empty();
}

void BFS::setDestination(const Vertex& destination)
{
	this->destination = &destination;
}

void BFS::checkIfDestination(const Vertex& vertex)
{
	if (&vertex == destination)
	{
		assert(!hasFoundAShortestPath);

		hasFoundAShortestPath = true;
	}
}
