#include "DirectedGraph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

static GraphRegistrator<DirectedGraph> registrator("directed");

DirectedGraph::DirectedGraph(const String& identifier) :
	GraphBase(identifier)
{
}

void DirectedGraph::addEdgeBetweenWithWeight(Vertex& start, Vertex& end, unsigned weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if (!hasEdgeFromTo(start, end))
	{
		addEdgeFromToWithWeight(start, end, weight);
	}
	else
	{
		throw GraphException("There already is such an edge in the graph!");
	}
}

void DirectedGraph::removeEdgeBetween(Vertex& start, Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	removeEdgeFromTo(start, end);
}
