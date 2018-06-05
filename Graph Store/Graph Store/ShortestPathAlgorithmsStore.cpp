#include "stdafx.h"
#include "ShortestPathAlgorithmsStore.h"

ShortestPathAlgorithmsStore::ShortestPathAlgorithmsStore() :
	algorithms(INITIAL_COLLECTION_SIZE)
{
}

ShortestPathAlgorithmsStore& ShortestPathAlgorithmsStore::instance()
{
	static ShortestPathAlgorithmsStore theOnlyInstance;
	return theOnlyInstance;
}

ShortestPathAlgorithm& ShortestPathAlgorithmsStore::searchForAlgorithm(const String& identifier)
{
	ShortestPathAlgorithm* algorithm = algorithms.search(identifier);

	if (algorithm != nullptr)
	{
		return *algorithm;
	}
	else
	{
		//
		//TODO: put an appropriate exception type here.
		//
		throw std::invalid_argument(identifier + " is not one of the supported algorithms!");
	}
}

void ShortestPathAlgorithmsStore::addAlgorithm(ShortestPathAlgorithm& algorithm)
{
	assert(algorithms.search(algorithm.getIdentifier()) == nullptr);

	algorithms.add(algorithm);
}
