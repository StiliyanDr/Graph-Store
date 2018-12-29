#include "DirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<DirectedGraph> registrator("directed");

DirectedGraph::DirectedGraph(const String& id) :
	Graph(id)
{
}

void DirectedGraph::addEdge(Vertex& start,
							Vertex& end,
							Edge::Weight weight)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	if (!hasEdge(start, end))
	{
		addEdgeFromTo(start, end, weight);
	}
	else
	{
		throw GraphException("There already is such an edge in the graph!"_s);
	}
}

void DirectedGraph::removeEdge(const Vertex& start,
							   const Vertex& end)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	removeEdgeFromTo(start, end);
}

void DirectedGraph::removeEdgesEndingIn(const Vertex& end)
{
	assert(isOwnerOf(end));

	VerticesConcreteIterator iterator =
		getConcreteIteratorOfVertices();

	forEach(iterator, [&](const Vertex& start)
	{
		try
		{
			removeEdgeFromTo(start, end);
		}
		catch (GraphException&)
		{
			//Ok, there is no such edge.
		}
	});
}

Graph::EdgesConstIterator
DirectedGraph::getConstIteratorOfEdges() const
{
	return createConstIteratorOfEdges<DirectedEdgesConstIterator>();
}
