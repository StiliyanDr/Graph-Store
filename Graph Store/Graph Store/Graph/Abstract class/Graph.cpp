#include "Graph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Iterator/ConcreteIteratorAdapter.h"

Graph::Edge::Edge(Vertex& start, OutgoingEdge& e) :
	start(start), incidentToStartEdge(e)
{
}

const Vertex& Graph::Edge::getStart() const
{
	return start;
}

const Vertex& Graph::Edge::getEnd() const
{
	return incidentToStartEdge.getVertex();
}

Graph::Edge::Weight Graph::Edge::getWeight() const
{
	return incidentToStartEdge.getWeight();
}

Graph::Graph(const String& id) :
	vertices(INITIAL_COLLECTION_SIZE),
	vertexSearchSet(INITIAL_COLLECTION_SIZE)
{
	setID(id);
}

void Graph::setID(String id)
{
	if (id != ""_s)
	{
		this->id = std::move(id);
	}
	else
	{
		throw GraphException("Invalid graph id: " + id);
	}
}

Graph::~Graph()
{
	destroyAllVertices();
}

void Graph::destroyAllVertices()
{
	VerticesConcreteIterator iterator = getConcreteIteratorOfVertices();

	forEach(iterator, [&](Vertex* v)
	{
		delete v;
	});
}

void Graph::removeVertex(Vertex& v)
{
	verifyOwnershipOf(v);

	removeEdgesEndingIn(v);
	removeEdgesLeaving(v);
	removeVertexFromCollection(v);

	delete &v;
}

void Graph::removeEdgeFromTo(Vertex& start, const Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	OutgoingEdgesConcreteIterator iteratorToEdge = searchForEdgeFromTo(start, end);

	if (iteratorToEdge)
	{
		getEdgesLeaving(start).removeAt(iteratorToEdge);
	}
	else
	{
		throw GraphException("There is no such edge!"_s);
	}
}

Graph::OutgoingEdgesConcreteIterator
Graph::searchForEdgeFromTo(Vertex& start, const Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	OutgoingEdgesConcreteIterator iterator =
		getConcreteIteratorOfEdgesLeaving(start);

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

bool Graph::hasEdge(const Vertex& start, const Vertex& end) const
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	const LinkedList<OutgoingEdge>& edges = start.edges;
	LinkedList<OutgoingEdge>::ConstIterator iterator = edges.getConstIterator();

	while (iterator)
	{
		if (iterator->getVertex() == end)
		{
			return true;
		}

		++iterator;
	}

	return false;
}

void Graph::removeEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	getEdgesLeaving(v).empty();
}

void Graph::removeVertexFromCollection(const Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	vertexSearchSet.remove(vertex.id);

	size_t indexOfLastVertex = vertices.getCount() - 1;
	assert(vertices[indexOfLastVertex]->index == indexOfLastVertex);

	vertices[indexOfLastVertex]->index = vertex.index;
	std::swap(vertices[indexOfLastVertex], vertices[vertex.index]);
	vertices.removeAt(indexOfLastVertex);
}

void Graph::addVertex(const String& id)
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

bool Graph::hasVertexWithID(const String& id) const
{
	return vertexSearchSet.contains(id);
}

void Graph::tryToAddNewVertex(const String& id)
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

std::unique_ptr<Vertex> Graph::createVertex(const String& id) const
{
	return std::unique_ptr<Vertex>(new Vertex(id, vertices.getCount()));
}

void Graph::addVertexToCollection(std::unique_ptr<Vertex> vertex)
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

Vertex& Graph::getVertexWithID(const String& id)
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

void Graph::addEdgeFromTo(Vertex& start, Vertex& end, OutgoingEdge::Weight weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	try
	{
		getEdgesLeaving(start).addFront(OutgoingEdge(&end, weight));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory!"_s);
	}
}

Graph::VerticesConstIterator Graph::getConstIteratorOfVertices() const
{
	typedef ConcreteIteratorAdapter<const Vertex*, DynamicArray<Vertex*>::ConstIterator, true> ConcreteConstIterator;

	return VerticesConstIterator(new ConcreteConstIterator(vertices.getConstIterator()));
}

Graph::VerticesConcreteIterator Graph::getConcreteIteratorOfVertices()
{
	return vertices.getIterator();
}

Graph::OutgoingEdgesConstIterator
Graph::getConstIteratorOfEdgesLeaving(const Vertex& v) const
{
	typedef ConcreteIteratorAdapter<OutgoingEdge, LinkedList<OutgoingEdge>::ConstIterator, true> ConcreteConstIterator;

	verifyOwnershipOf(v);
	const LinkedList<OutgoingEdge>& edges = v.edges;

	return OutgoingEdgesConstIterator(new ConcreteConstIterator(edges.getConstIterator()));
}

Graph::OutgoingEdgesConcreteIterator
Graph::getConcreteIteratorOfEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	return getEdgesLeaving(v).getIteratorToFirst();
}

void Graph::verifyOwnershipOf(const Vertex& v) const
{
	if (!isOwnerOf(v))
	{
		throw GraphException("Received a vertex from another graph!"_s);
	}
}

bool Graph::isOwnerOf(const Vertex& v) const
{
	return v.index < vertices.getCount() && vertices[v.index] == &v;
}

LinkedList<OutgoingEdge>& Graph::getEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	return v.edges;
}

unsigned Graph::getVerticesCount() const
{
	return vertices.getCount();
}

const String& Graph::getID() const
{
	return id;
}
