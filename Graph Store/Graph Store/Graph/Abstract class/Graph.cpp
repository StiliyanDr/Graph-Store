#include "Graph.h"
#include "Graph/Graph Exception/GraphException.h"

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

void Graph::Vertex::exchangeContentsWith(Vertex& v)
{
	std::swap(id, v.id);
	std::swap(index, v.index);
	std::swap(iterator, v.iterator);
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
	vertexSearchSet(INITIAL_COLLECTION_SIZE)
{
	vertices.reserve(INITIAL_COLLECTION_SIZE);
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

	OutgoingEdgesConcreteIterator iteratorToEdge =
		searchForEdgeFromTo(start, end);

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

	return findIf(iterator, [&](const OutgoingEdge& e)
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

	return anyOf(iterator, [&](const OutgoingEdge& e)
	{
		return e.getEnd() == end;
	});
}

void Graph::removeEdgesLeaving(const Vertex& v)
{
	assert(isOwnerOf(v));

	AdjacencyListsIterator iterator = v.iterator;
	adjacencyLists.removeAt(iterator);
}

void Graph::removeVertexFromCollection(Vertex& v)
{
	assert(isOwnerOf(v));

	vertexSearchSet.remove(v.id);
	
	size_t indexOfLastVertex = getVerticesCount() - 1;
	vertexSearchSet.remove(vertices[indexOfLastVertex].id);
	
	size_t indexToRemoveAt = v.index;
	vertices[indexOfLastVertex].index = indexToRemoveAt;
	vertices[indexToRemoveAt].exchangeContentsWith(vertices[indexOfLastVertex]);
	vertices.pop_back();

	vertexSearchSet.add(vertices[indexToRemoveAt]);
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
		addVertexToCollection(createVertex(id));
	}
	catch (std::bad_alloc&)
	{
		throw GraphException("Not enough memory for a new vertex!"_s);
	}
}

Graph::Vertex Graph::createVertex(String id)
{
	try
	{
		return Vertex(std::move(id),
		              getVerticesCount(),
		              createAdjacencyListAndReturnIterator());
	}
	catch (GraphException&)
	{
		removeNewlyCreatedAdjacencyList();
		throw;
	}
}

Graph::AdjacencyListsIterator
Graph::createAdjacencyListAndReturnIterator()
{
	adjacencyLists.addFront(AdjacencyList());
	
	return adjacencyLists.getIteratorToFirst();
}

void Graph::removeNewlyCreatedAdjacencyList()
{
	adjacencyLists.removeFirst();
}

void Graph::addVertexToCollection(const Vertex& v)
{
	assert(v.iterator == adjacencyLists.getIteratorToFirst());

	try
	{
		vertices.push_back(v);

		try
		{
			vertexSearchSet.add(vertices.back());
		}
		catch (std::bad_alloc&)
		{
			vertices.pop_back();
			throw;
		}
	}
	catch (std::bad_alloc&)
	{
		removeNewlyCreatedAdjacencyList();
		throw;
	}
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

void Graph::addEdgeFromTo(const Vertex& start,
						  Vertex& end,
						  OutgoingEdge::Weight weight)
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

Graph::VerticesConstIterator
Graph::getConstIteratorOfVertices() const
{
	return std::make_unique<VerticesConcreteConstIterator>(getConcreteConstIteratorOfVertices());
}

Graph::VerticesConcreteIterator
Graph::getConcreteIteratorOfVertices()
{
	return VerticesConcreteIterator(vertices.begin(), vertices.end());
}

Graph::VerticesConcreteConstIterator
Graph::getConcreteConstIteratorOfVertices() const
{
	return VerticesConcreteConstIterator(vertices.cbegin(), vertices.cend());
}

Graph::OutgoingEdgesConstIterator
Graph::getConstIteratorOfEdgesLeaving(const Vertex& v) const
{
	verifyOwnershipOf(v);

	return std::make_unique<AdjacencyList::ConstIterator>(
        getConcreteConstIteratorOfEdgesLeaving(v)
    );
}

Graph::OutgoingEdgesConcreteIterator
Graph::getConcreteIteratorOfEdgesLeaving(const Vertex& v)
{
	assert(isOwnerOf(v));

	return getEdgesLeaving(v).getIteratorToFirst();
}

Graph::OutgoingEdgesConcreteConstIterator
Graph::getConcreteConstIteratorOfEdgesLeaving(const Vertex& v) const
{
	assert(isOwnerOf(v));

	return getEdgesLeaving(v).getConstIterator();
}

void Graph::verifyNoSuchEdgeExists(const Vertex& start,
								   const Vertex& end) const
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if (hasEdge(start, end))
	{
		throw GraphException("There already is such an edge in the graph!"_s);
	}
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
	return v.index < getVerticesCount() && vertices[v.index] == v;
}

Graph::AdjacencyList&
Graph::getEdgesLeaving(const Vertex& v)
{
	assert(isOwnerOf(v));

	return *(v.iterator);
}

const Graph::AdjacencyList&
Graph::getEdgesLeaving(const Vertex& v) const
{
	assert(isOwnerOf(v));

	return *(v.iterator);
}

unsigned Graph::getVerticesCount() const
{
	return vertices.size();
}

unsigned Graph::getEdgesCount() const
{
	unsigned count = 0;

	forEach(adjacencyLists.getConstIterator(), [&](const AdjacencyList& edges)
	{
		count += edges.getSize();
	});

	return count;
}

const String& Graph::getID() const
{
	return id;
}
