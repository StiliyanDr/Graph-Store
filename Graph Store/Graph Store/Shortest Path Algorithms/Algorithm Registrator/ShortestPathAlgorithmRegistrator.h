#ifndef __SHORTEST_PATH_ALGORITHM_REGISTRATOR_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_REGISTRATOR_HEADER_INCLUDED__

#include "../Store/ShortestPathAlgorithmsStore.h"

template <class Algorithm>
class ShortestPathAlgorithmRegistrator
{
public:
	explicit ShortestPathAlgorithmRegistrator(const char* algorithmID) :
		algorithm(algorithmID)
	{
		ShortestPathAlgorithmsStore::instance().addAlgorithm(algorithm);
	}

	ShortestPathAlgorithmRegistrator(const ShortestPathAlgorithmRegistrator&) = delete;
	ShortestPathAlgorithmRegistrator&
		operator=(const ShortestPathAlgorithmRegistrator&) = delete;

private:
	Algorithm algorithm;
};

#endif //__SHORTEST_PATH_ALGORITHM_REGISTRATOR_HEADER_INCLUDED__