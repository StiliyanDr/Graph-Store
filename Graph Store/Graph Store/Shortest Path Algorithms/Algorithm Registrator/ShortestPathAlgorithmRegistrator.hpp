#include "../Store/ShortestPathAlgorithmsStore.h"

template <class Algorithm>
ShortestPathAlgorithmRegistrator<Algorithm>::ShortestPathAlgorithmRegistrator(const char* algorithmID) :
	algorithm(algorithmID)
{
	ShortestPathAlgorithmsStore::instance().addAlgorithm(algorithm);
}
