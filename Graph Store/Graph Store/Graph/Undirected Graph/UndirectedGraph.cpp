#include "UndirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");

UndirectedGraph::UndirectedGraph(const String& identifier) :
	GraphBase(identifier)
{
}

void UndirectedGraph::addEdgeBetweenWithWeight(Vertex& start, Vertex& end, unsigned weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if (!hasEdgeFromTo(start, end))
	{
		addEdgeFromToWithWeight(start, end, weight);
		addEdgeFromToWithWeight(end, start, weight);
	}
	else
	{
		throw GraphException("There already is such an edge in the graph!");
	}
}

void UndirectedGraph::removeEdgeBetween(Vertex& start, Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	removeEdgeFromTo(start, end);
	removeEdgeFromTo(end, start);
}

void UndirectedGraph::removeEdgesEndingIn(Vertex& vertex)
{
	EdgeAbstractIterator iterator = getIteratorOfEdgesStartingFrom(vertex);

	while (iterator->isValid())
	{
		Edge& edge = iterator->getCurrentItem();
		Vertex& endOfEdge = edge.getVertex();
		
		removeEdgeFromTo(endOfEdge, vertex);
		
		iterator->advance();
	}
}
