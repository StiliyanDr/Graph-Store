#include "Graph.h"
#include "../Graph Exception/GraphException.h"
#include "../../Iterator/ConcreteIteratorAdapter.h"

Graph::Vertex::Vertex(String id,
	                  size_t index,
	                  const AdjacencyListsIterator& iterator) :
	index(index),
	iterator(iterator)
{
	setID(std::move(id));
}

void Graph::Vertex::setID(String id)
{
	if (id != ""_s)
	{
		this->id = std::move(id);
	}
	else
	{
		throw GraphException("Invalid vertex id: " + id);
	}
}

const String& Graph::Vertex::getID() const
{
	return id;
}

bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs)
{
	return !(lhs == rhs);
}

bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs)
{
	return &lhs == &rhs;
}

Graph::OutgoingEdge::OutgoingEdge(Vertex& end, Weight w) :
	weight(w),
	end(end)
{
}

Graph::OutgoingEdge::Weight Graph::OutgoingEdge::getWeight() const
{
	return weight;
}

const Graph::Vertex& Graph::OutgoingEdge::getEnd() const
{
	return end;
}

Graph::Vertex& Graph::OutgoingEdge::getEnd()
{
	return end;
}

Graph::Edge::Edge(const Vertex& start, const OutgoingEdge& e) :
	start(start), incidentToStartEdge(e)
{
}

const Graph::Vertex& Graph::Edge::getStart() const
{
	return start;
}

const Graph::Vertex& Graph::Edge::getEnd() const
{
	return incidentToStartEdge.getEnd();
}

Graph::Edge::Weight Graph::Edge::getWeight() const
{
	return incidentToStartEdge.getWeight();
}

Graph::EdgesConstIteratorBase::EdgesConstIteratorBase(const VerticesConcreteConstIterator& verticesIterator,
	                                                  const OutgoingEdgesConcreteConstIterator& edgesIterator) :
	verticesIterator(verticesIterator),
	edgesIterator(edgesIterator)
{
}

Graph::EdgesConstIteratorBase&
Graph::EdgesConstIteratorBase::operator++()
{
	++edgesIterator;
	skipIteratedEdges();

	return *this;
}

void Graph::EdgesConstIteratorBase::skipIteratedEdges()
{
	while (thereAreMoreEdges() && !pointsToUniteratedEdge())
	{
		goToNextEdge();
	}
}

bool Graph::EdgesConstIteratorBase::thereAreMoreEdges() const
{
	return static_cast<bool>(verticesIterator);
}

bool Graph::EdgesConstIteratorBase::pointsToUniteratedEdge() const
{
	return pointsToEdge()
		   && verticesIterator->index < edgesIterator->getEnd().index;
}

bool Graph::EdgesConstIteratorBase::pointsToEdge() const
{
	return static_cast<bool>(verticesIterator)
		   && static_cast<bool>(edgesIterator);
}

void Graph::EdgesConstIteratorBase::goToNextEdge()
{
	++edgesIterator;
	goToNextListIfCurrentOneEnded();
}

void Graph::EdgesConstIteratorBase::goToNextListIfCurrentOneEnded()
{
	if (!edgesIterator)
	{
		++verticesIterator;

		if (verticesIterator)
		{
			const AdjacencyList& edges = *(verticesIterator->iterator);
			edgesIterator = edges.getConstIterator();
		}
	}
}

Graph::Edge Graph::EdgesConstIteratorBase::operator*() const
{
	if (pointsToEdge())
	{
		assert(pointsToUniteratedEdge());

		return Edge(*verticesIterator, *edgesIterator);
	}
	else
	{
		throw std::out_of_range("Iterator out of range!");
	}
}

Graph::EdgesConstIteratorBase::operator bool() const
{
	return pointsToEdge();
}

bool Graph::EdgesConstIteratorBase::operator!() const
{
	return !pointsToEdge();
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

void Graph::removeVertex(Vertex& v)
{
	verifyOwnershipOf(v);

	removeEdgesEndingIn(v);
	removeEdgesLeaving(v);
	removeVertexFromCollection(v);
}

void Graph::removeEdgeFromTo(const Vertex& start, const Vertex& end)
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
Graph::searchForEdgeFromTo(const Vertex& start, const Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	OutgoingEdgesConcreteIterator iterator =
		getConcreteIteratorOfEdgesLeaving(start);

	return forEachUntil(iterator, [&](const OutgoingEdge& e)
	{
		return e.getEnd() == end;
	});
}

bool Graph::hasEdge(const Vertex& start, const Vertex& end) const
{
	verifyOwnershipOf(start);
	verifyOwnershipOf(end);

	OutgoingEdgesConcreteConstIterator iterator =
		getConcreteConstIteratorOfEdgesLeaving(start);

	return forEachUntil(iterator, [&](const OutgoingEdge& e)
	{
		return e.getEnd() == end;
	});
}

void Graph::removeEdgesLeaving(const Vertex& v)
{
	assert(isOwnerOf(v));

	Vertex::AdjacencyListsIterator iterator = v.iterator;
	adjacencyLists.removeAt(iterator);
}

void Graph::removeVertexFromCollection(Vertex& v)
{
	assert(isOwnerOf(v));

	vertexSearchSet.remove(v.id);

	size_t indexOfLastVertex = getVerticesCount() - 1;
	size_t indexToRemoveAt = v.index;

	std::swap(vertices[indexToRemoveAt], vertices[indexOfLastVertex]);

	vertexSearchSet.remove(vertices[indexToRemoveAt].id);
	vertexSearchSet.add(vertices[indexToRemoveAt]);

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

std::unique_ptr<Graph::Vertex> Graph::createVertex(const String& id) const
{
	return std::make_unique<Vertex>(id, vertices.getCount());
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

Graph::Vertex& Graph::getVertexWithID(const String& id)
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
		getEdgesLeaving(start).addFront(OutgoingEdge(end, weight));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory!"_s);
	}
}

Graph::VerticesConstIterator Graph::getConstIteratorOfVertices() const
{
	typedef ConcreteConstIteratorAdapter<const Vertex*, DynamicArray<Vertex*>::ConstIterator> ConcreteConstIterator;

	return std::make_unique<ConcreteConstIterator>(getConcreteConstIteratorOfVertices());
}

Graph::VerticesConcreteIterator Graph::getConcreteIteratorOfVertices()
{
	return vertices.getIterator();
}

Graph::VerticesConcreteConstIterator
Graph::getConcreteConstIteratorOfVertices() const
{
	return vertices.getConstIterator();
}

Graph::OutgoingEdgesConstIterator
Graph::getConstIteratorOfEdgesLeaving(const Vertex& v) const
{
	typedef ConcreteConstIteratorAdapter<OutgoingEdge, LinkedList<OutgoingEdge>::ConstIterator> ConcreteConstIterator;

	verifyOwnershipOf(v);

	return std::make_unique<ConcreteConstIterator>(getConcreteConstIteratorOfEdgesLeaving(v));
}

Graph::OutgoingEdgesConcreteIterator
Graph::getConcreteIteratorOfEdgesLeaving(Vertex& v)
{
	assert(isOwnerOf(v));

	return getEdgesLeaving(v).getIteratorToFirst();
}

Graph::OutgoingEdgesConcreteConstIterator
Graph::getConcreteConstIteratorOfEdgesLeaving(const Vertex& v) const
{
	assert(isOwnerOf(v));

	return v.edges.getConstIterator();
}

template <class ConcreteIterator>
typename Graph::EdgesConstIterator
Graph::createConstIteratorOfEdges() const
{
	VerticesConcreteConstIterator verticesIterator =
		getConcreteConstIteratorOfVertices();
	OutgoingEdgesConcreteConstIterator edgesIterator =
		verticesIterator ? getConcreteConstIteratorOfEdgesLeaving(*verticesIterator) :
		                   AdjacencyList().getConstIterator();

	return std::make_unique<ConcreteIterator>(verticesIterator, edgesIterator);
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

LinkedList<Graph::OutgoingEdge>& Graph::getEdgesLeaving(Vertex& v)
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
