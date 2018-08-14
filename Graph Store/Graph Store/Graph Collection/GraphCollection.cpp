#include "GraphCollection.h"

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
	CollectionIterator iterator = graphs.getIteratorToFirst();

	while (iterator.isValid())
	{
		delete iterator.getCurrentItem();

		iterator.advance();
	}
}

GraphCollection::~GraphCollection()
{
	destroyAllGraphs();
}

void GraphCollection::add(std::unique_ptr<Graph> graph)
{
	const String& id = graph->getIdentifier();

	if (!hasGraphWithID(id))
	{
		tryToAdd(std::move(graph));
	}
	else
	{
		throw std::runtime_error("There already is a graph with id: " + id);
	}
}

bool GraphCollection::hasGraphWithID(const String& id) const
{
	size_t count = graphs.getCount();

	for (size_t i = 0; i < count; ++i)
	{
		if (graphs[i]->getIdentifier() == id)
		{
			return true;
		}
	}

	return false;
}

void GraphCollection::tryToAdd(std::unique_ptr<Graph> graph)
{
	try
	{
		graphs.add(graph.get());
	}
	catch (std::bad_alloc&)
	{
		throw std::runtime_error("Not enough memory to add a graph!");
	}

	graph.release();
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
		if (graphs[i]->getIdentifier() == id)
		{
			return i;
		}
	}

	throw std::runtime_error("There is no graph with id: " + id);
}

Graph& GraphCollection::getGraphWithID(const String& id)
{
	size_t index = getIndexOfGraphWithID(id);
	
	return *graphs[index];
}

std::unique_ptr<Iterator<Graph*>> GraphCollection::getIterator()
{
	CollectionIterator iterator = graphs.getIteratorToFirst();

	return std::unique_ptr<Iterator<Graph*>>(new CollectionIterator(iterator));
}

bool GraphCollection::isEmpty() const
{
	return getCount() == 0;
}

size_t GraphCollection::getCount() const
{
	return graphs.getCount();
}
