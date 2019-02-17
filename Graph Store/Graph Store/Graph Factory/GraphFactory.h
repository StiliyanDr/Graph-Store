#ifndef __GRAPH_FACTORY_HEADER_INCLUDED__
#define __GRAPH_FACTORY_HEADER_INCLUDED__

#include "../Graph Factory/Graph Creator/GraphCreator.h"
#include "../Dynamic Array/DynamicArray.h"
#include "../Graph/Abstract class/Graph.h"

class GraphFactory
{
	template <class ConcreteGraph>
	friend class GraphRegistrator;

	typedef DynamicArray<const GraphCreator*> Collection;
	typedef DynamicArray<const GraphCreator*>::ConstIterator ConstIterator;

public:
	static GraphFactory& instance();

public:
	std::unique_ptr<Graph> createGraph(const String& criterion, const String& id);

private:
	GraphFactory();
	GraphFactory(const GraphFactory&);
	GraphFactory& operator=(const GraphFactory&);
	GraphFactory(GraphFactory&&);
	GraphFactory& operator=(GraphFactory&&);
	~GraphFactory() = default;

	const GraphCreator& getCreatorByCriterion(const String& criterion);
	Collection::ConstIterator searchForCreator(const String& criterion) const;
	void addCreator(const GraphCreator& c);

private:
	static const std::size_t INITIAL_COLLECTION_SIZE = 4;

private:
	Collection creators;
};

#endif //__GRAPH_FACTORY_HEADER_INCLUDED__