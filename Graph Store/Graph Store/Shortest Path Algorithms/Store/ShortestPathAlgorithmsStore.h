#ifndef __SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "../../Hash/Identifier Accessor/IdentifierAccessor.h"
#include "../../Hash/Hash.h"
#include "../../Hash/Hash Function/HashFunctionStringSpecialization.h"

class ShortestPathAlgorithmsStore
{
	using Collection = Hash<ShortestPathAlgorithm, String, IdentifierAccessor>;

public:
	static ShortestPathAlgorithmsStore& instance();

	ShortestPathAlgorithm& search(const String& id);
	void addAlgorithm(ShortestPathAlgorithm& a);

private:
	ShortestPathAlgorithmsStore();
	ShortestPathAlgorithmsStore(const ShortestPathAlgorithmsStore&);
	ShortestPathAlgorithmsStore& operator=(const ShortestPathAlgorithmsStore&);
	~ShortestPathAlgorithmsStore() = default;

private:
	static const std::size_t INITIAL_COLLECTION_SIZE = 4;

private:
	Collection algorithms;
};

#endif //__SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__