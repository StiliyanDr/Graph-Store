#include "ShortestPathAlgorithmsStore.h"
#include "Runtime Error/RuntimeError.h"
#include "String/String.h"
#include "Shortest Path Algorithms/Abstract class/ShortestPathAlgorithm.h"
#include "Iterator/Iterator.h"
#include <assert.h>

ShortestPathAlgorithmsStore::IDComparator::IDComparator(const String& id) noexcept :
	id(id)
{
}

bool
ShortestPathAlgorithmsStore::IDComparator::operator()(const ShortestPathAlgorithm* a) const
{
	assert(a != nullptr);

	return a->getID() == id;
}

ShortestPathAlgorithmsStore::ShortestPathAlgorithmsStore() :
	algorithms(INITIAL_COLLECTION_SIZE)
{
}

ShortestPathAlgorithmsStore&
ShortestPathAlgorithmsStore::instance()
{
	static ShortestPathAlgorithmsStore theOnlyInstance;
	
	return theOnlyInstance;
}

ShortestPathAlgorithm&
ShortestPathAlgorithmsStore::operator[](const String& id)
{
	auto iterator =
		findIf(algorithms.getIterator(), IDComparator{ id });

	if (iterator)
	{
		return *(*iterator);
	}
	else
	{
		throw RuntimeError{ id + " is not one of the supported algorithms!" };
	}
}

void ShortestPathAlgorithmsStore::addAlgorithm(ShortestPathAlgorithm& a)
{
	if (!contains(a.getID()))
	{
		algorithms.add(&a);
	}
	else
	{
		throw std::logic_error{ "Duplicate algorithm!" };
	}
}

bool ShortestPathAlgorithmsStore::contains(const String& id) const
{
	return anyOf(algorithms.getConstIterator(),
		         IDComparator{ id });
}
