#ifndef __GRAPH_FACTORY_HEADER_INCLUDED__
#define __GRAPH_FACTORY_HEADER_INCLUDED__

#include "Dynamic Array/DynamicArray.h"
#include <memory>

class String;
class Graph;
class GraphCreator;

class GraphFactory
{
	template <class ConcreteGraph>
	friend class GraphRegistrator;

	using Collection = DynamicArray<const GraphCreator*>;

public:
	static GraphFactory& instance();

public:
	std::unique_ptr<Graph> createGraph(const String& criterion,
		                               const String& id) const;

private:
	GraphFactory();
	GraphFactory(const GraphFactory&);
	GraphFactory& operator=(const GraphFactory&);
	GraphFactory(GraphFactory&&);
	GraphFactory& operator=(GraphFactory&&);
	~GraphFactory() = default;

	const GraphCreator& getCreator(const String& criterion) const;
	Collection::ConstIterator searchForCreator(const String& criterion) const;
	void addCreator(const GraphCreator& c);

private:
	static const std::size_t INITIAL_COLLECTION_SIZE = 4;

private:
	Collection creators;
};

#endif //__GRAPH_FACTORY_HEADER_INCLUDED__