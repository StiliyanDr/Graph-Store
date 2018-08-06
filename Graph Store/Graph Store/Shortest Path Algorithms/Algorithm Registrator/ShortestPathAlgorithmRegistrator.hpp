#include "../Store/ShortestPathAlgorithmsStore.h"

template <class Algorithm>
ShortestPathAlgorithmRegistrator<Algorithm>::ShortestPathAlgorithmRegistrator(const char* algorithmIdentifier) :
	algorithm(algorithmIdentifier)
{
	ShortestPathAlgorithmsStore::instance().addAlgorithm(algorithm);
}
