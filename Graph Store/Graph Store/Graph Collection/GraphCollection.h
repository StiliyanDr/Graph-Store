#ifndef __GRAPH_COLLECTION_HEADER_INCLUDED__
#define __GRAPH_COLLECTION_HEADER_INCLUDED__

#include "../Dynamic Array/DynamicArray.h"
#include "../Graph/Abstract class/Graph.h"
#include "../Iterator/Iterator.h"

class GraphCollection
{
	using Collection = DynamicArray<Graph*>;

public:
	GraphCollection() = default;
	GraphCollection(const GraphCollection&) = delete;
	GraphCollection& operator=(const GraphCollection&) = delete;
	GraphCollection(GraphCollection&&) = default;
	GraphCollection& operator=(GraphCollection&& rhs);
	~GraphCollection();

	void add(Graph& graph);
	std::unique_ptr<Graph> remove(const String& graphID);
	Graph& getGraphWithID(const String& id);
	
	std::unique_ptr<Iterator<Graph*>> getIterator();
	size_t getCount() const;
	bool isEmpty() const;
	void empty();

private:
	bool hasGraphWithID(const String& id) const;
	void tryToAdd(Graph& graph);
	size_t getIndexOfGraphWithID(const String& id) const;
	void destroyAllGraphs();

private:
	Collection graphs;
};

#endif //__GRAPH_COLLECTION_HEADER_INCLUDED__