#include "SearchBasedShortestPathAlgorithm.h"
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

void SearchBasedShortestPathAlgorithm::addDecoratedVersionOf(const Vertex& v)
{
	decoratedVertices.emplace(v.getID(), v);
}

ShortestPathAlgorithm::DecoratedVertex&
SearchBasedShortestPathAlgorithm::getDecoratedVersionOf(const Vertex& v)
{
	return decoratedVertices[v.getID()];
}

void SearchBasedShortestPathAlgorithm::visitVertex(MarkableDecoratedVertex& successor,
												   const MarkableDecoratedVertex& predecessor)
{
	successor.parent = &predecessor;
	successor.distanceToSource = predecessor.distanceToSource + 1;
}

void SearchBasedShortestPathAlgorithm::checkIfTarget(const MarkableDecoratedVertex& v)
{
	if (v.originalVertex == *target)
	{
		assert(!foundAShortestPath);

		foundAShortestPath = true;
	}
}

void SearchBasedShortestPathAlgorithm::setTarget(const Vertex& target)
{
	this->target = &target;
}
