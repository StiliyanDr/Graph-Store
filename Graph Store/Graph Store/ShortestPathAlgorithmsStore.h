#ifndef __SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__

#include "ShortestPathAlgorithm.h"
#include "IdentifierAccessor.h"
#include "Hash.h"
#include "HashFunctionStringSpecialization.h"

class ShortestPathAlgorithmsStore
{
	typedef Hash<ShortestPathAlgorithm, String, IdentifierAccessor> Collection;

public:
	static ShortestPathAlgorithmsStore& instance();

	ShortestPathAlgorithm& searchForAlgorithm(const String& identifier);
	void addAlgorithm(ShortestPathAlgorithm& algorithm);

private:
	ShortestPathAlgorithmsStore();
	ShortestPathAlgorithmsStore(const ShortestPathAlgorithmsStore&);
	ShortestPathAlgorithmsStore& operator=(const ShortestPathAlgorithmsStore&);
	~ShortestPathAlgorithmsStore() = default;

private:
	static const size_t INITIAL_COLLECTION_SIZE = 4;

private:
	Collection algorithms;
};

#endif //__SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__