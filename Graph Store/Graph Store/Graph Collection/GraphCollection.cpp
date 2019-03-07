#include "GraphCollection.h"
#include "Runtime Error\RuntimeError.h"
#include <algorithm>

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
	Collection::const_iterator iterator = getGraph(id);
	
	return *(*iterator);
}

bool GraphCollection::contains(const String& id) const
{
	return findGraph(id) != graphs.cend();
}

GraphCollection::Collection::const_iterator
GraphCollection::getGraph(const String& id) const
{
	Collection::const_iterator iterator = findGraph(id);

	if (iterator != graphs.cend())
	{
		return iterator;
	}
	else
	{
		std::string message = "There is no graph with id: ";
		throw std::out_of_range(message + id.cString());
	}
}

GraphCollection::Collection::const_iterator
GraphCollection::findGraph(const String& id) const
{
	return std::find_if(graphs.cbegin(),
	                    graphs.cend(),
		                [&](const GraphPointer& graph)
	{
		assert(graph != nullptr);

		return graph->getID() == id;
	});
}

void GraphCollection::empty()
{
	graphs.clear();
}

void GraphCollection::remove(const String& id)
{
	Collection::const_iterator graphToRemove = getGraph(id);
	graphs.erase(graphToRemove);
}
