#include "GraphBase.h"
#include "../Graph Exception/GraphException.h"

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
	VertexConcreteIterator iterator = getConcreteIteratorOfVertices();

	while (iterator.isValid())
	{
		destroyVertex(*iterator);
		++iterator;
	}
}

void GraphBase::removeVertex(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	removeEdgesEndingIn(vertex);
	removeEdgesStartingFrom(vertex);
	removeVertexFromCollection(vertex);
	destroyVertex(&vertex);
}

void GraphBase::removeEdgesEndingIn(Vertex& vertex)
{
	VertexConcreteIterator iterator = getConcreteIteratorOfVertices();

	while (iterator.isValid())
	{
		try
		{
			removeEdgeFromTo(*(*iterator), vertex);
		}
		catch (GraphException&)
		{
			//Ok, there is no such edge.
		}

		++iterator;
	}
}

void GraphBase::removeEdgeFromTo(Vertex& start, const Vertex& end)
{
	EdgeConcreteIterator iteratorToEdge = searchForEdgeFromTo(start, end);

	if (iteratorToEdge.isValid())
	{
		getEdgesStartingFrom(start).removeAt(iteratorToEdge);
	}
	else
	{
		throw GraphException("There is no such edge!");
	}
}

GraphBase::EdgeConcreteIterator GraphBase::searchForEdgeFromTo(Vertex& start, const Vertex& end)
{
	EdgeConcreteIterator iterator = getConcreteIteratorOfEdgesStartingFrom(start);

	while (iterator.isValid())
	{
		Vertex& endOfEdge = (*iterator).getVertex();

		if (endOfEdge.index == end.index)
		{
			break;
		}

		++iterator;
	}

	return iterator;
}

bool GraphBase::hasEdgeFromTo(Vertex& start, const Vertex& end)
{
	EdgeConcreteIterator iteratorToEdge = searchForEdgeFromTo(start, end);

	return iteratorToEdge.isValid();
}

void GraphBase::removeEdgesStartingFrom(Vertex& vertex)
{
	getEdgesStartingFrom(vertex).empty();
}

void GraphBase::removeVertexFromCollection(const Vertex& vertex)
{
	assert(vertices[vertex.index] == &vertex);

	vertexSearchSet.remove(vertex.identifier);

	size_t indexOfLastVertex = vertices.getCount() - 1;
	assert(vertices[indexOfLastVertex]->index == indexOfLastVertex);

	vertices[indexOfLastVertex]->index = vertex.index;
	std::swap(vertices[indexOfLastVertex], vertices[vertex.index]);
	vertices.removeAt(indexOfLastVertex);
}

void GraphBase::addVertex(const String& identifier)
{
	if (!hasVertexWithIdentifier(identifier))
	{
		tryToAddNewVertex(identifier);
	}
	else
	{
		throw GraphException("There already is a vertex with identifier: " + identifier);
	}
}

bool GraphBase::hasVertexWithIdentifier(const String& identifier)
{
	return searchForVertexWithIdentifier(identifier) != nullptr;
}

Vertex* GraphBase::searchForVertexWithIdentifier(const String& identifier)
{
	return vertexSearchSet.search(identifier);
}

void GraphBase::tryToAddNewVertex(const String& identifier)
{
	try
	{
		std::unique_ptr<Vertex> newVertex = createVertex(identifier);
		addVertexToCollection(std::move(newVertex));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory for a new vertex!");
	}
}

std::unique_ptr<Vertex> GraphBase::createVertex(const String& identifier) const
{
	return std::unique_ptr<Vertex>(new Vertex(identifier, vertices.getCount()));
}

void GraphBase::addVertexToCollection(std::unique_ptr<Vertex> vertex)
{
	assert(vertex != nullptr);
	assert(vertex->index == vertices.getCount());

	vertices.add(vertex.get());

	try
	{
		vertexSearchSet.add(*vertex);
	}
	catch (std::bad_alloc&)
	{
		vertices.removeAt(vertex->index);
		throw;
	}

	vertex.release();
}

Vertex& GraphBase::getVertexWithIdentifier(const String& identifier)
{
	Vertex* vertex = searchForVertexWithIdentifier(identifier);

	if (vertex != nullptr)
	{
		return *vertex;
	}
	else
	{
		throw GraphException("There is no vertex with identifier: " + identifier);
	}
}

void GraphBase::addEdgeFromToWithWeight(Vertex& start, Vertex& end, unsigned weight)
{
	getEdgesStartingFrom(start).addFront(Edge(&end, weight));
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

bool GraphBase::isOwnerOf(const Vertex& vertex) const
{
	return vertex.index < vertices.getCount() && vertices[vertex.index] == &vertex;
}

void GraphBase::destroyVertex(Vertex* vertex) const
{
	delete vertex;
}

LinkedList<Edge>& GraphBase::getEdgesStartingFrom(Vertex& vertex)
{
	return vertex.edges;
}

unsigned GraphBase::getVerticesCount() const
{
	return vertices.getCount();
}
