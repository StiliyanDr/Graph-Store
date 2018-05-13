#include "stdafx.h"
#include "UndirectedGraph.h"
#include <assert.h>
#include <stdexcept>

UndirectedGraph::UndirectedGraph(const char* identifier) :
	GraphBase(identifier)
{
}

void UndirectedGraph::addEdgeBetweenWithWeight(Vertex& startVertex, Vertex& endVertex, unsigned weight)
{
	assert(isOwnerOf(startVertex));
	assert(isOwnerOf(endVertex));

	if (!hasEdgeFromTo(startVertex, endVertex))
	{
		addEdgeFromToWithWeight(startVertex, endVertex, weight);
		addEdgeFromToWithWeight(endVertex, startVertex, weight);
	}
	else
	{
		throw std::logic_error("There already is such an edge in the graph!");
	}
}

void UndirectedGraph::removeEdgeBetween(Vertex& startVertex, Vertex& endVertex)
{
	assert(isOwnerOf(startVertex));
	assert(isOwnerOf(endVertex));

	removeEdgeFromTo(startVertex, endVertex);
	removeEdgeFromTo(endVertex, startVertex);
}

void UndirectedGraph::removeEdgesEndingIn(Vertex& vertex)
{
	EdgeAbstractIterator edgeIterator = getIteratorOfEdgesStartingFrom(vertex);

	while (edgeIterator->isValid())
	{
		Vertex& endOfEdge = edgeIterator->getCurrentItem().getVertex();
		removeEdgeFromTo(endOfEdge, vertex);
		edgeIterator->advance();
	}
}
