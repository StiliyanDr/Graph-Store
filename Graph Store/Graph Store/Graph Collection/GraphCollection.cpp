#include "GraphCollection.h"
#include "Runtime Error\RuntimeError.h"

GraphCollection::GraphCollection(std::size_t size) :
	graphs(size)
{
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
