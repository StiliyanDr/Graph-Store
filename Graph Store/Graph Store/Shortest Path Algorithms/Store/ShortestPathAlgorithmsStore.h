#ifndef __SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__

#include "Dynamic Array/DynamicArray.h"

class String;
class ShortestPathAlgorithm;

class ShortestPathAlgorithmsStore
{
	using Collection = DynamicArray<ShortestPathAlgorithm*>;

	class IDComparator
	{
	public:
		explicit IDComparator(const String& id) noexcept;
		bool operator()(const ShortestPathAlgorithm* a) const;

	private:
		const String& id;
	};

public:
	static ShortestPathAlgorithmsStore& instance();

public:
	ShortestPathAlgorithmsStore(const ShortestPathAlgorithmsStore&) = delete;
	ShortestPathAlgorithmsStore&
		operator=(const ShortestPathAlgorithmsStore&) = delete;
	ShortestPathAlgorithm& operator[](const String& id);
	void addAlgorithm(ShortestPathAlgorithm& a);
	bool contains(const String& id) const;

private:
	ShortestPathAlgorithmsStore();
	~ShortestPathAlgorithmsStore() = default;

private:
	static const std::size_t INITIAL_COLLECTION_SIZE = 4u;

private:
	Collection algorithms;
};

#endif //__SHORTEST_PATH_ALGORITHMS_STORE_HEADER_INCLUDED__