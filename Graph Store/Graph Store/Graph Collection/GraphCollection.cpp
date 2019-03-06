#include "GraphCollection.h"

GraphCollection::GraphCollection(std::size_t size) :
	graphs(size)
{
}

bool GraphCollection::isEmpty() const noexcept
{
	return getCount() == 0;
}

std::size_t GraphCollection::getCount() const noexcept
{
	return graphs.getCount();
}
