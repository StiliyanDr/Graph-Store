#include "BFSShortestPath.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<BFSShortestPath> registrator("bfs");

BFSShortestPath::BFSShortestPath(const char* id) :
	ShortestPathAlgorithm(id)
{
}

void BFSShortestPath::findShortestPath(Graph& graph, Vertex& source, Vertex& target)
{
	initialiseAlgorithm(graph, source, target);

	Vertex* vertex;
	std::unique_ptr<Iterator<Edge>> iterator;

	while (!(hasFoundAShortestPath || theFrontierIsEmpty()))
	{
		vertex = getNextVertexFromFrontier();
		iterator = graph.getIteratorOfEdgesStartingFrom(*vertex);

		forEach(*iterator, [&](Edge& edge)
		{
			exploreEdge(*vertex, edge.getVertex());
		});
	}

	cleanUpAlgorithmState();
}

void BFSShortestPath::initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& target)
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

void BFSShortestPath::initialiseVertex(Vertex& vertex) const
{
	ShortestPathAlgorithm::initialiseVertex(vertex);
}

void BFSShortestPath::initialiseSource(Vertex& source) const
{
	source.markAsVisited();
	ShortestPathAlgorithm::initialiseSource(source);
}

void BFSShortestPath::buildTrivialPathStartingFrom(Vertex& vertex)
{
	vertex.setParent(nullptr);
	vertex.setDistance(0);
}

void BFSShortestPath::exploreEdge(Vertex& predecessor, Vertex& successor)
{
	if (successor.isMarkedAsVisited())
	{
		return;
	}

	visitVertex(successor, predecessor);
	addVertexToFrontier(successor);
	checkIfTarget(successor);
}

void BFSShortestPath::visitVertex(Vertex& vertex, Vertex& predecessor) const
{
	assert(!vertex.isMarkedAsVisited());

	vertex.markAsVisited();
	vertex.setParent(&predecessor);
	vertex.setDistance(predecessor.getDistance() + 1);
}

void BFSShortestPath::checkIfTarget(const Vertex& vertex)
{
	if (vertex == *target)
	{
		assert(!hasFoundAShortestPath);

		hasFoundAShortestPath = true;
	}
}

void BFSShortestPath::addVertexToFrontier(Vertex& vertex)
{
	queue.enqueue(&vertex);
}

bool BFSShortestPath::theFrontierIsEmpty() const
{
	return queue.isEmpty();
}

Vertex* BFSShortestPath::getNextVertexFromFrontier()
{
	return queue.dequeue();
}

void BFSShortestPath::cleanUpAlgorithmState()
{
	queue.empty();
}

void BFSShortestPath::setTarget(const Vertex& target)
{
	this->target = &target;
}
