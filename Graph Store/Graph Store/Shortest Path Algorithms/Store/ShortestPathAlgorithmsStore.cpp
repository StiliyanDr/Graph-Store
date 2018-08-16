#include "ShortestPathAlgorithmsStore.h"
#include "Unsupported Alogirhtm Exception/UnsupportedAlgorithmException.h"

ShortestPathAlgorithmsStore::ShortestPathAlgorithmsStore() :
	algorithms(INITIAL_COLLECTION_SIZE)
{
}

ShortestPathAlgorithmsStore& ShortestPathAlgorithmsStore::instance()
{
	static ShortestPathAlgorithmsStore theOnlyInstance;
	
	return theOnlyInstance;
}

ShortestPathAlgorithm& ShortestPathAlgorithmsStore::searchForAlgorithm(const String& id)
{
	ShortestPathAlgorithm* algorithm = algorithms.search(id);

	if (algorithm != nullptr)
	{
		return *algorithm;
	}
	else
	{
		throw UnsupportedAlgorithmException(id + " is not one of the supported algorithms!");
	}
}

void ShortestPathAlgorithmsStore::addAlgorithm(ShortestPathAlgorithm& algorithm)
{
	assert(algorithms.search(algorithm.getID()) == nullptr);

	algorithms.add(algorithm);
}
