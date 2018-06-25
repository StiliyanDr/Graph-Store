#include "stdafx.h"
#include "GraphBase.h"
#include <stdexcept>
#include <assert.h>
#include <utility>

GraphBase::GraphBase(String identifier) :
	Graph(std::move(identifier)),
	vertices(INITIAL_COLLECTION_SIZE),
	vertexSearchSet(INITIAL_COLLECTION_SIZE)
{
}

GraphBase::~GraphBase()
{
	destroyAllVertices();
}

void GraphBase::destroyAllVertices()
{
	VertexConcreteIterator vertexIterator = getConcreteIteratorOfVertices();

	while (vertexIterator.isValid())
	{
		destroyVertex(*vertexIterator);
		++vertexIterator;
	}
}

void GraphBase::addVertex(const char* identifier)
{
	if (!hasVertexWithIdentifier(identifier))
	{
		Vertex* newVertex = createVertex(identifier);
		addVertexToCollection(newVertex);
	}
	else
	{
		throw std::logic_error("There already is a vertex with that identifier in the graph!");
	}
}

bool GraphBase::hasVertexWithIdentifier(const char* identifier)
{
	return getVertexWithIdentifier(identifier) != nullptr;
}

Vertex* GraphBase::getVertexWithIdentifier(const char* identifier)
{
	return vertexSearchSet.search(identifier);
}

Vertex* GraphBase::createVertex(const char* identifier) const
{
	return new Vertex(identifier, vertices.getCount());
}

void GraphBase::addVertexToCollection(Vertex* vertex)
{
	assert(vertex != nullptr);
	assert(vertex->index == vertices.getCount());

	vertices.add(vertex);
	vertexSearchSet.add(*vertex);
}

void GraphBase::removeVertex(Vertex& vertexToRemove)
{
	assert(isOwnerOf(vertexToRemove));

	removeEdgesEndingIn(vertexToRemove);
	removeEdgesStartingFrom(vertexToRemove);
	removeVertexFromCollection(vertexToRemove);
	destroyVertex(&vertexToRemove);
}

void GraphBase::removeEdgesEndingIn(Vertex& vertex)
{
	VertexConcreteIterator vertexIterator = getConcreteIteratorOfVertices();

	while (vertexIterator.isValid())
	{
		try
		{
			removeEdgeFromTo(*(*vertexIterator), vertex);
		}
		catch (std::logic_error&)
		{
			//Ok, there is no such edge.
		}

		++vertexIterator;
	}
}

void GraphBase::removeEdgeFromTo(Vertex& startVertex, const Vertex& endVertex)
{
	EdgeConcreteIterator iteratorToEdge = searchForEdgeFromTo(startVertex, endVertex);

	if (iteratorToEdge.isValid())
	{
		getEdgesStartingFrom(startVertex).removeAt(iteratorToEdge);
	}
	else
	{
		throw std::logic_error("There is no such edge!");
	}
}

GraphBase::EdgeConcreteIterator GraphBase::searchForEdgeFromTo(Vertex& startVertex, const Vertex& endVertex)
{
	EdgeConcreteIterator edgeIterator = getConcreteIteratorOfEdgesStartingFrom(startVertex);

	while (edgeIterator.isValid())
	{
		const Vertex& endOfEdge = (*edgeIterator).getVertex();

		if (endOfEdge.index == endVertex.index)
		{
			break;
		}

		++edgeIterator;
	}

	return edgeIterator;
}

void GraphBase::removeEdgesStartingFrom(Vertex& vertex)
{
	getEdgesStartingFrom(vertex).empty();
}

void GraphBase::removeVertexFromCollection(const Vertex& vertexToRemove)
{
	assert(vertices[vertexToRemove.index] == &vertexToRemove);

	vertexSearchSet.remove(vertexToRemove.identifier);

	const size_t indexOfLastVertex = vertices.getCount() - 1;
	assert(vertices[indexOfLastVertex]->index == indexOfLastVertex);
	
	vertices[indexOfLastVertex]->index = vertexToRemove.index;
	std::swap(vertices[indexOfLastVertex], vertices[vertexToRemove.index]);
	vertices.removeAt(indexOfLastVertex);
}

void GraphBase::destroyVertex(Vertex* vertex) const
{
	delete vertex;
}

bool GraphBase::hasEdgeFromTo(Vertex& startVertex, const Vertex& endVertex)
{
	EdgeConcreteIterator iteratorToEdge = searchForEdgeFromTo(startVertex, endVertex);

	return iteratorToEdge.isValid();
}

void GraphBase::addEdgeFromToWithWeight(Vertex& startVertex, Vertex& endVertex, unsigned weight)
{
	getEdgesStartingFrom(startVertex).addFront(Edge(&endVertex, weight));
}

bool GraphBase::isOwnerOf(const Vertex& vertex) const
{
	return vertex.index < vertices.getCount() && vertices[vertex.index] == &vertex;
}

Graph::VertexAbstractIterator GraphBase::getIteratorOfVertices()
{
	return VertexAbstractIterator(new VertexConcreteIterator(getConcreteIteratorOfVertices()));
}

GraphBase::VertexConcreteIterator GraphBase::getConcreteIteratorOfVertices()
{
	return vertices.getIteratorToFirst();
}

Graph::EdgeAbstractIterator GraphBase::getIteratorOfEdgesStartingFrom(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	return EdgeAbstractIterator(new EdgeConcreteIterator(getConcreteIteratorOfEdgesStartingFrom(vertex)));
}

GraphBase::EdgeConcreteIterator GraphBase::getConcreteIteratorOfEdgesStartingFrom(Vertex& vertex)
{
	return getEdgesStartingFrom(vertex).getIteratorToFirst();
}

LinkedList<Edge>& GraphBase::getEdgesStartingFrom(Vertex& vertex)
{
	return vertex.edges;
}

unsigned GraphBase::getVerticesCount() const
{
	return vertices.getCount();
}
