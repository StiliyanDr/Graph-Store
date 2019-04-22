#include "GraphCollection.h"
#include "Runtime Error/RuntimeError.h"

GraphCollection::GraphCollection(std::size_t size)
{
	graphs.reserve(size);
}

void GraphCollection::add(GraphPointer graph)
{
	verifyPointerIsNotNull(graph);

	const String& id = graph->getID();

	if (!contains(id))
	{
		tryToAdd(std::move(graph));
	}
	else
	{
		throw RuntimeError("There already is a graph with id: " + id);
	}
}

void GraphCollection::tryToAdd(GraphPointer graph)
{
	try
	{
		graphs.push_back(std::move(graph));
	}
	catch (std::bad_alloc&)
	{
		throw RuntimeError("Not enough memory!"_s);
	}
}

void GraphCollection::verifyPointerIsNotNull(const GraphPointer& p)
{
	if (p == nullptr)
	{
		throw RuntimeError("Expected a graph!"_s);
	}
}

bool GraphCollection::isEmpty() const noexcept
{
	return getCount() == 0;
}

std::size_t GraphCollection::getCount() const noexcept
{
	return graphs.size();
}

Graph& GraphCollection::operator[](const String& id)
{
	const GraphCollection& collection = *this;

	return const_cast<Graph&>(collection[id]);
}

const Graph&
GraphCollection::operator[](const String& id) const
{
	Collection::const_iterator iterator =
		getGraph(graphs.cbegin(), graphs.cend(), id);
	
	return *(*iterator);
}

bool GraphCollection::contains(const String& id) const
{
	return findGraph(graphs.cbegin(),
		             graphs.cend(),
		             id) != graphs.cend();
}

void GraphCollection::empty()
{
	graphs.clear();
}

GraphCollection::GraphPointer
GraphCollection::remove(const String& id)
{
	Collection::iterator iterator =
		getGraph(graphs.begin(), graphs.end(), id);

	GraphPointer graphToRemove = std::move(*iterator);
	graphs.erase(iterator);
	
	return graphToRemove;
}

GraphCollection::Iterator
GraphCollection::getIterator() noexcept
{
	return Iterator(graphs.begin(), graphs.end());
}

GraphCollection::ConstIterator
GraphCollection::getConstIterator() const noexcept
{
	return ConstIterator(graphs.cbegin(), graphs.cend());
}
