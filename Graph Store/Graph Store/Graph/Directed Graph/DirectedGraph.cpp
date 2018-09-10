#include "DirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<DirectedGraph> registrator("directed");

DirectedGraph::DirectedGraph(const String& id) :
	GraphBase(id)
{
}

void DirectedGraph::addEdge(Vertex& start, Vertex& end, unsigned weight)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	if (!hasEdgeFromTo(start, end))
	{
		addEdgeFromTo(start, end, weight);
	}
	else
	{
		throw GraphException("There already is such an edge in the graph!");
	}
}

void DirectedGraph::removeEdge(Vertex& start, Vertex& end)
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	removeEdgeFromTo(start, end);
}

void DirectedGraph::removeEdgesEndingIn(Vertex& end)
{
	assert(isOwnerOf(end));

	VerticesConcreteIterator iterator = getConcreteIteratorOfVertices();

	forEach(iterator, [&](Vertex* start)
	{
		try
		{
			removeEdgeFromTo(*start, end);
		}
		catch (GraphException&)
		{
			//Ok, there is no such edge.
		}
	});
}
