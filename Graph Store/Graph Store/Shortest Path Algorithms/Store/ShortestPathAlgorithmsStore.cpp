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

ShortestPathAlgorithm& ShortestPathAlgorithmsStore::search(const String& id)
{
	try
	{
		return algorithms[id];
	}
	catch (std::logic_error&)
	{
		throw UnsupportedAlgorithmException(id + " is not one of the supported algorithms!");
	}
}

void ShortestPathAlgorithmsStore::addAlgorithm(ShortestPathAlgorithm& algorithm)
{
	assert(!algorithms.contains(algorithm.getID()));

	algorithms.add(algorithm);
}
