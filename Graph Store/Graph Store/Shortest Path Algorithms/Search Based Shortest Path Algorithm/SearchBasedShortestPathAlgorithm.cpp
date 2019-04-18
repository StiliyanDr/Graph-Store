#include "SearchBasedShortestPathAlgorithm.h"
#include "Graph/Abstract class/Graph.h"
#include <assert.h>

SearchBasedShortestPathAlgorithm::SearchBasedShortestPathAlgorithm(const String& id) :
	ShortestPathAlgorithm(id)
{
}

void SearchBasedShortestPathAlgorithm::decorateVerticesOf(const Graph& graph)
{
	decoratedVertices.reserve(graph.getVerticesCount());

	ShortestPathAlgorithm::decorateVerticesOf(graph);
}

void SearchBasedShortestPathAlgorithm::addDecoratedVersionOf(const Graph::Vertex& v)
{
	decoratedVertices.emplace(v.getID(), v);
}

SearchBasedShortestPathAlgorithm::MarkableDecoratedVertex&
SearchBasedShortestPathAlgorithm::getDecoratedVersionOf(const Graph::Vertex& v)
{
	return decoratedVertices[v.getID()];
}

void SearchBasedShortestPathAlgorithm::cleanUp()
{
	decoratedVertices.clear();
}

void SearchBasedShortestPathAlgorithm::visitVertex(MarkableDecoratedVertex& successor,
												   const MarkableDecoratedVertex& predecessor)
{
	successor.parent = &predecessor;
	successor.distanceToSource = predecessor.distanceToSource + 1;
}

void SearchBasedShortestPathAlgorithm::checkIfTarget(const MarkableDecoratedVertex& v)
{
	if (*(v.originalVertex) == *target)
	{
		assert(!foundAShortestPath);

		foundAShortestPath = true;
	}
}

void SearchBasedShortestPathAlgorithm::setTarget(const Graph::Vertex& target)
{
	this->target = &target;
}
