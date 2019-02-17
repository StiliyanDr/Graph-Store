#ifndef __CONCRETE_GRAPH_CREATOR_HEADER_INCLUDED__
#define __CONCRETE_GRAPH_CREATOR_HEADER_INCLUDED__

#include "GraphCreator.h"

template <class ConcreteGraph>
class ConcreteGraphCreator : public GraphCreator
{
public:
	explicit ConcreteGraphCreator(const char* criterion) :
		GraphCreator(criterion)
	{
	}

	virtual std::unique_ptr<Graph> createGraph(const String& id) const override
	{
		return std::make_unique<ConcreteGraph>(id);
	}
};

#endif //__CONCRETE_GRAPH_CREATOR_HEADER_INCLUDED__