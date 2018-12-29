#include "UndirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");

UndirectedGraph::UndirectedGraph(const String& id) :
	Graph(id)
{
}

void UndirectedGraph::addEdge(Vertex& start,
							  Vertex& end,
							  Edge::Weight weight)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);
	verifyEdgeWouldNotBeALoop(start, end);
	verifyNoSuchEdgeExists(start, end);
	tryToAddUndirectedEdge(start, end, weight);
}

void UndirectedGraph::verifyEdgeWouldNotBeALoop(const Vertex& start,
												const Vertex& end) const
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if (start == end)
	{
		throw GraphException("Proposed edge is a loop!"_s);
	}
}

void UndirectedGraph::tryToAddUndirectedEdge(Vertex& start,
											 Vertex& end,
											 Edge::Weight weight)
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

void UndirectedGraph::removeEdge(const Vertex& start, const Vertex& end)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	removeEdgeFromTo(start, end);
	removeEdgeFromTo(end, start);
}

void UndirectedGraph::removeEdgesEndingIn(const Vertex& v)
{
	OutgoingEdgesConcreteIterator iterator =
		getConcreteIteratorOfEdgesLeaving(v);

	forEach(iterator, [&](const OutgoingEdge& e)
	{
		removeEdgeFromTo(e.getEnd(), v);
	});
}

unsigned UndirectedGraph::getEdgesCount() const
{
	return Graph::getEdgesCount() / 2;
}

Graph::EdgesConstIterator
UndirectedGraph::getConstIteratorOfEdges() const
{
	return createConstIteratorOfEdges<EdgesConstIteratorBase>();
}
