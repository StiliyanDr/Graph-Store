#include "BFS.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<BFS> registrator("bfs");

BFS::BFS(const char* identifier) :
	ShortestPathAlgorithm(identifier)
{
}

void BFS::findShortestPath(Graph& graph, Vertex& source, Vertex& target)
{
	initialiseAlgorithm(graph, source, target);

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

void BFS::initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& target)
{
	assert(theFrontierIsEmpty());

	if (source != target)
	{
		hasFoundAShortestPath = false;
		setTarget(target);
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

void BFS::initialiseVertex(Vertex& vertex) const
{
	ShortestPathAlgorithm::initialiseVertex(vertex);
}

void BFS::initialiseSource(Vertex& source) const
{
	source.markAsVisited();
	ShortestPathAlgorithm::initialiseSource(source);
}

void BFS::buildTrivialPathStartingFrom(Vertex& vertex)
{
	vertex.setParent(nullptr);
	vertex.setDistance(0);
}

void BFS::exploreEdge(Vertex& predecessor, Vertex& successor)
{
	if (successor.isMarkedAsVisited())
	{
		return;
	}

	visitVertex(successor, predecessor);
	addVertexToFrontier(successor);
	checkIfTarget(successor);
}

void BFS::visitVertex(Vertex& vertex, Vertex& predecessor) const
{
	assert(!vertex.isMarkedAsVisited());

	vertex.markAsVisited();
	vertex.setParent(&predecessor);
	vertex.setDistance(predecessor.getDistance() + 1);
}

void BFS::checkIfTarget(const Vertex& vertex)
{
	if (vertex == *target)
	{
		assert(!hasFoundAShortestPath);

		hasFoundAShortestPath = true;
	}
}

void BFS::addVertexToFrontier(Vertex& vertex)
{
	queue.enqueue(&vertex);
}

bool BFS::theFrontierIsEmpty() const
{
	return queue.isEmpty();
}

Vertex* BFS::getNextVertexFromFrontier()
{
	return queue.dequeue();
}

void BFS::cleanUpAlgorithmState()
{
	queue.empty();
}

void BFS::setTarget(const Vertex& target)
{
	this->target = &target;
}
