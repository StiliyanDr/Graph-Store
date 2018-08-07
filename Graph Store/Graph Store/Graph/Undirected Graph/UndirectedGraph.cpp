#include "UndirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");

UndirectedGraph::UndirectedGraph(const String& identifier) :
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
		throw GraphException("There already is such an edge in the graph!");
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
