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
	typedef DynamicArrayIterator<const GraphCreator*> Iterator;

public:
	static std::unique_ptr<Graph> createGraph(const String& criterion, const String& identifier);

private:
	static const GraphCreator& getCreatorByCriterion(const String& criterion);
	static const GraphCreator* searchForCreator(const String& criterion);
	static void addCreator(const GraphCreator& creator);
	static Collection& getCreators();

private:
	static const size_t INITIAL_COLLECTION_SIZE = 4;
};

#endif //__GRAPH_FACTORY_HEADER_INCLUDED__