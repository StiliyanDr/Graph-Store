#include "GraphBase.h"
#include "../Graph Exception/GraphException.h"
#include "../../Iterator/ConcreteIteratorAdapter.h"

GraphBase::GraphBase(const String& id) :
	Graph(id),
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
	VerticesConcreteIterator iterator = getConcreteIteratorOfVertices();

	forEach(iterator, [&](Vertex* v)
	{
		delete v;
	});
}

void GraphBase::removeVertex(Vertex& v)
{
	verifyOwnershipOf(v);

	removeEdgesEndingIn(v);
	removeEdgesLeaving(v);
	removeVertexFromCollection(v);
	
	delete &v;
}

void GraphBase::removeEdgeFromTo(Vertex& start, const Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	EdgesConcreteIterator iteratorToEdge = searchForEdgeFromTo(start, end);

	if (iteratorToEdge)
	{
		getEdgesLeaving(start).removeAt(iteratorToEdge);
	}
	else
	{
		throw GraphException("There is no such edge!"_s);
	}
}

GraphBase::EdgesConcreteIterator GraphBase::searchForEdgeFromTo(Vertex& start, const Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	EdgesConcreteIterator iterator = getConcreteIteratorOfEdgesLeaving(start);

	while (iterator)
	{
		if (iterator->getVertex() == end)
		{
			break;
		}

		++iterator;
	}

	return iterator;
}

bool GraphBase::hasEdgeFromTo(const Vertex& start, const Vertex& end) const
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	const LinkedList<Edge>& edges = start.edges;
	LinkedList<Edge>::ConstIterator iterator = edges.getConstIterator();

	while(iterator)
	{
		if (iterator->getVertex() == end)
		{
			return true;
		}

		++iterator;
	}

	return false;
}

void GraphBase::removeEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	getEdgesLeaving(v).empty();
}

void GraphBase::removeVertexFromCollection(const Vertex& vertex)
{
	assert(isOwnerOf(vertex));

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
		throw GraphException("There already is a vertex with id: " + id);
	}
}

bool GraphBase::hasVertexWithID(const String& id) const
{
	return vertexSearchSet.contains(id);
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
		throw GraphException("Not enough memory for a new vertex!"_s);
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
	try
	{
		return vertexSearchSet[id];
	}
	catch (std::logic_error&)
	{
		throw GraphException("There is no vertex with id: " + id);
	}
}

void GraphBase::addEdgeFromTo(Vertex& start, Vertex& end, unsigned weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	try
	{
		getEdgesLeaving(start).addFront(Edge(&end, weight));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory!"_s);
	}
}

Graph::VerticesConstIterator GraphBase::getConstIteratorOfVertices() const
{
	typedef ConcreteIteratorAdapter<const Vertex*, DynamicArray<Vertex*>::ConstIterator, true> ConcreteConstIterator;

	return VerticesConstIterator(new ConcreteConstIterator(vertices.getConstIterator()));
}

GraphBase::VerticesConcreteIterator GraphBase::getConcreteIteratorOfVertices()
{
	return vertices.getIterator();
}

Graph::EdgesConstIterator GraphBase::getConstIteratorOfEdgesLeaving(const Vertex& v) const
{
	typedef ConcreteIteratorAdapter<Edge, LinkedList<Edge>::ConstIterator, true> ConcreteConstIterator;

	verifyOwnershipOf(v);
	const LinkedList<Edge>& edges = v.edges;

	return EdgesConstIterator(new ConcreteConstIterator(edges.getConstIterator()));
}

GraphBase::EdgesConcreteIterator GraphBase::getConcreteIteratorOfEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	return getEdgesLeaving(v).getIteratorToFirst();
}

void GraphBase::verifyOwnershipOf(const Vertex& v) const
{
	if (!isOwnerOf(v))
	{
		throw GraphException("Received a vertex from another graph!"_s);
	}
}

bool GraphBase::isOwnerOf(const Vertex& v) const
{
	return v.index < vertices.getCount() && vertices[v.index] == &v;
}

LinkedList<Edge>& GraphBase::getEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	return v.edges;
}

unsigned GraphBase::getVerticesCount() const
{
	return vertices.getCount();
}
