#include "UndirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");

UndirectedGraph::UndirectedGraph(const String& id) :
	GraphBase(id)
{
}

void UndirectedGraph::addEdge(Vertex& start, Vertex& end, unsigned weight)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	if (!hasEdgeFromTo(start, end))
	{
		tryToAddUndirectedEdge(start, end, weight);
	}
	else
	{
		throw GraphException("There already is such an edge in the graph!"_s);
	}
}

void UndirectedGraph::tryToAddUndirectedEdge(Vertex& start, Vertex& end, unsigned weight)
{
	addEdgeFromTo(start, end, weight);

	try
	{
		addEdgeFromTo(end, start, weight);
	}
	catch (GraphException&)
	{
		removeEdgeFromTo(start, end);
		throw;
	}
}

void UndirectedGraph::removeEdge(Vertex& start, Vertex& end)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	removeEdgeFromTo(start, end);
	removeEdgeFromTo(end, start);
}

void UndirectedGraph::removeEdgesEndingIn(Vertex& v)
{
	EdgesConcreteIterator iterator = getConcreteIteratorOfEdgesLeaving(v);

	forEach(iterator, [&](Edge& e)
	{
		Vertex& endOfEdge = e.getVertex();

		removeEdgeFromTo(endOfEdge, v);
	});
}
