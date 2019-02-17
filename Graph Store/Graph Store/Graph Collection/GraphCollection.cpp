#include "GraphCollection.h"
#include "../Runtime Error/RuntimeError.h"
#include "../Iterator/ConcreteIteratorAdapter.h"

GraphCollection& GraphCollection::operator=(GraphCollection&& rhs)
{
	if (this != &rhs)
	{
		empty();
		graphs = std::move(rhs.graphs);
	}

	return *this;
}

void GraphCollection::empty()
{
	destroyAllGraphs();
	graphs.empty();
}

void GraphCollection::destroyAllGraphs()
{
	Collection::Iterator iterator = graphs.getIterator();

	forEach(iterator, [&](Graph* graph)
	{
		delete graph;
	});
}

GraphCollection::~GraphCollection()
{
	destroyAllGraphs();
}

void GraphCollection::add(Graph& graph)
{
	const String& id = graph.getID();

	if (!hasGraphWithID(id))
	{
		tryToAdd(graph);
	}
	else
	{
		throw RuntimeError("There already is a graph with id: " + id);
	}
}

bool GraphCollection::hasGraphWithID(const String& id) const
{
	size_t count = graphs.getCount();

	for (size_t i = 0; i < count; ++i)
	{
		if (graphs[i]->getID() == id)
		{
			return true;
		}
	}

	return false;
}

void GraphCollection::tryToAdd(Graph& graph)
{
	try
	{
		graphs.add(&graph);
	}
	catch (std::bad_alloc&)
	{
		throw RuntimeError("Not enough memory to add a graph!"_s);
	}
}

std::unique_ptr<Graph> GraphCollection::remove(const String& graphID)
{
	size_t index = getIndexOfGraphWithID(graphID);
	
	std::unique_ptr<Graph> graph(graphs[index]);
	graphs.removeAt(index);

	return graph;
}

size_t GraphCollection::getIndexOfGraphWithID(const String& id) const
{
	size_t count = graphs.getCount();

	for (size_t i = 0; i < count; ++i)
	{
		if (graphs[i]->getID() == id)
		{
			return i;
		}
	}

	throw RuntimeError("There is no graph with id: " + id);
}

Graph& GraphCollection::getGraphWithID(const String& id)
{
	size_t index = getIndexOfGraphWithID(id);
	
	return *graphs[index];
}

std::unique_ptr<Iterator<Graph*>> GraphCollection::getIterator()
{
	using ConcreteIterator = ConcreteIteratorAdapter<Graph*, Collection::Iterator>;

	return std::make_unique<ConcreteIterator>(graphs.getIterator());
}

bool GraphCollection::isEmpty() const
{
	return getCount() == 0;
}

size_t GraphCollection::getCount() const
{
	return graphs.getCount();
}
