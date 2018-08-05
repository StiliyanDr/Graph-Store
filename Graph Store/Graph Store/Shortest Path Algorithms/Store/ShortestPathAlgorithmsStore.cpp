#include "ShortestPathAlgorithmsStore.h"
#include "../Store/Unsupported Alogirhtm Exception/UnsupportedAlgorithmException.h"

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
		throw UnsupportedAlgorithmException(identifier + " is not one of the supported algorithms!");
	}
}

void ShortestPathAlgorithmsStore::addAlgorithm(ShortestPathAlgorithm& algorithm)
{
	assert(algorithms.search(algorithm.getIdentifier()) == nullptr);

	algorithms.add(algorithm);
}
