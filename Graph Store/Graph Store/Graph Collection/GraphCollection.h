#ifndef __GRAPH_COLLECTION_HEADER_INCLUDED__
#define __GRAPH_COLLECTION_HEADER_INCLUDED__

#include "Graph\Abstract class\Graph.h"
#include <vector>
#include <memory>

class GraphCollection
{
public:
	template <bool isConst = false>
	class GraphCollectionIterator
	{
		///
		///TODO
		///
	};

	using Iterator = GraphCollectionIterator<false>;
	using ConstIterator = GraphCollectionIterator<true>;

	using GraphPointer = std::unique_ptr<Graph>;

private:
	using Collection = std::vector<GraphPointer>;

public:
	GraphCollection() = default;
	explicit GraphCollection(std::size_t size);
	GraphCollection(const GraphCollection&) = delete;
	GraphCollection& operator=(const GraphCollection&) = delete;
	GraphCollection(GraphCollection&&) = default;
	GraphCollection& operator=(GraphCollection&&) = default;

	void add(GraphPointer graph);
	GraphPointer remove(const String& id);

	Graph& operator[](const String& id);
	const Graph& operator[](const String& id) const;
	bool contains(const String& id) const noexcept;

	Iterator getIterator() noexcept;
	ConstIterator getConstIterator() const noexcept;

	std::size_t getCount() const noexcept;
	bool isEmpty() const noexcept;
	void empty();

private:
	template <class CollectionIterator>
	static CollectionIterator findGraph(CollectionIterator iterator,
		                                const String& id);
	template <class CollectionIterator>
	static void verifyExistenceOfGraph(CollectionIterator iterator,
		                               const String& id);
	static void verifyPointerIsNotNull(const GraphPointer& p);

private:
	void tryToAdd(GraphPointer graph);

private:
	Collection graphs;
};

#endif //__GRAPH_COLLECTION_HEADER_INCLUDED__