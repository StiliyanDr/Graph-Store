#ifndef __SHORTEST_PATH_ALGORITHM_REGISTRATOR_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_REGISTRATOR_HEADER_INCLUDED__

template <class Algorithm>
class ShortestPathAlgorithmRegistrator
{
public:
	ShortestPathAlgorithmRegistrator(const char* algorithmIdentifier);
	ShortestPathAlgorithmRegistrator(const ShortestPathAlgorithmRegistrator<Algorithm>&) = delete;
	ShortestPathAlgorithmRegistrator<Algorithm>& operator=(const ShortestPathAlgorithmRegistrator<Algorithm>&) = delete;
	ShortestPathAlgorithmRegistrator(ShortestPathAlgorithmRegistrator<Algorithm>&&) = delete;
	ShortestPathAlgorithmRegistrator<Algorithm>& operator=(ShortestPathAlgorithmRegistrator<Algorithm>&&) = delete;
	~ShortestPathAlgorithmRegistrator() = default;

private:
	Algorithm algorithm;
};

#include "ShortestPathAlgorithmRegistrator.hpp"

#endif //__SHORTEST_PATH_ALGORITHM_REGISTRATOR_HEADER_INCLUDED__