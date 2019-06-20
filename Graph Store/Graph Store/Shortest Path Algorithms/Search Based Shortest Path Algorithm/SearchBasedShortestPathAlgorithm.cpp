#include "SearchBasedShortestPathAlgorithm.h"
#include <assert.h>

SearchBasedShortestPathAlgorithm::SearchBasedShortestPathAlgorithm(String id) :
	ShortestPathAlgorithm(std::move(id)),
	foundAShortestPath(false),
	target(nullptr)
{
}

void SearchBasedShortestPathAlgorithm::decorateVerticesOf(const Graph& g)
{
	decoratedVertices.reserve(g.getVerticesCount());

	ShortestPathAlgorithm::decorateVerticesOf(g);
}

void SearchBasedShortestPathAlgorithm::addDecoratedVersionOf(const Graph::Vertex& v)
{
	decoratedVertices.emplace(v.getID(), v);
}

SearchBasedShortestPathAlgorithm::MarkableDecoratedVertex&
SearchBasedShortestPathAlgorithm::getDecoratedVersionOf(const Graph::Vertex& v)
{
	return decoratedVertices.at(v.getID());
}

void SearchBasedShortestPathAlgorithm::cleanUp()
{
	foundAShortestPath = false;
	target = nullptr;
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

void SearchBasedShortestPathAlgorithm::setTarget(const Graph::Vertex& v)
{
	target = &v;
}
