#include "GraphBase.h"
#include "../Graph Exception/GraphException.h"

GraphBase::GraphBase(String id) :
	Graph(std::move(id)),
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

	forEach(iterator, [&](Vertex* v)
	{
		destroyVertex(v);
	});
}

void GraphBase::removeVertex(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	removeEdgesEndingIn(vertex);
	removeEdgesStartingFrom(vertex);
	removeVertexFromCollection(vertex);
	destroyVertex(&vertex);
}

void GraphBase::removeEdgesEndingIn(Vertex& end)
{
	VertexConcreteIterator iterator = getConcreteIteratorOfVertices();

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

void GraphBase::removeEdgeFromTo(Vertex& start, const Vertex& end)
{
	EdgeConcreteIterator iteratorToEdge = searchForEdgeFromTo(start, end);

	if (iteratorToEdge)
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

	while (iterator)
	{
		Vertex& endOfEdge = iterator->getVertex();

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

	return static_cast<bool>(iteratorToEdge);
}

void GraphBase::removeEdgesStartingFrom(Vertex& vertex)
{
	getEdgesStartingFrom(vertex).empty();
}

void GraphBase::removeVertexFromCollection(const Vertex& vertex)
{
	assert(vertices[vertex.index] == &vertex);

	vertexSearchSet.remove(vertex.id);

	size_t indexOfLastVertex = vertices.getCount() - 1;
	assert(vertices[indexOfLastVertex]->index == indexOfLastVertex);

	vertices[indexOfLastVertex]->index = vertex.index;
	std::swap(vertices[indexOfLastVertex], vertices[vertex.index]);
	vertices.removeAt(indexOfLastVertex);
}

void GraphBase::addVertex(const String& id)
{
	if (!hasVertexWithID(id))
	{
		tryToAddNewVertex(id);
	}
	else
	{
		throw GraphException("There already is a vertex with identifier: " + id);
	}
}

bool GraphBase::hasVertexWithID(const String& id)
{
	return searchForVertexWithID(id) != nullptr;
}

Vertex* GraphBase::searchForVertexWithID(const String& id)
{
	return vertexSearchSet.search(id);
}

void GraphBase::tryToAddNewVertex(const String& id)
{
	try
	{
		std::unique_ptr<Vertex> newVertex = createVertex(id);
		addVertexToCollection(std::move(newVertex));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory for a new vertex!");
	}
}

std::unique_ptr<Vertex> GraphBase::createVertex(const String& id) const
{
	return std::unique_ptr<Vertex>(new Vertex(id, vertices.getCount()));
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

Vertex& GraphBase::getVertexWithID(const String& id)
{
	Vertex* vertex = searchForVertexWithID(id);

	if (vertex != nullptr)
	{
		return *vertex;
	}
	else
	{
		throw GraphException("There is no vertex with identifier: " + id);
	}
}

void GraphBase::addEdgeFromToWithWeight(Vertex& start, Vertex& end, unsigned weight)
{
	try
	{
		getEdgesStartingFrom(start).addFront(Edge(&end, weight));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory!");
	}
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
