#include "UndirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");

UndirectedGraph::UndirectedGraph(const String& id) :
	GraphBase(id)
{
}

void UndirectedGraph::addEdgeBetweenWithWeight(Vertex& start, Vertex& end, unsigned weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if (!hasEdgeFromTo(start, end))
	{
		tryToAddUndirectedEdge(start, end, weight);
	}
	else
	{
		throw GraphException("There already is such an edge in the graph!");
	}
}

void UndirectedGraph::tryToAddUndirectedEdge(Vertex& start, Vertex& end, unsigned weight)
{
	addEdgeFromToWithWeight(start, end, weight);

	try
	{
		addEdgeFromToWithWeight(end, start, weight);
	}
	catch (GraphException&)
	{
		removeEdgeFromTo(start, end);
		throw;
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

	forEach(*iterator, [&](Edge& edge)
	{
		Vertex& endOfEdge = edge.getVertex();

		removeEdgeFromTo(endOfEdge, vertex);
	});
}
