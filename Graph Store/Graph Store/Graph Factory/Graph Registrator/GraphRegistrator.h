#ifndef __GRAPH_REGISTRATOR_HEADER_INCLUDED__
#define __GRAPH_REGISTRATOR_HEADER_INCLUDED__

#include "Graph Factory/Graph Creator/ConcreteGraphCreator.h"
#include "Graph Factory/GraphFactory.h"

template <class ConcreteGraph>
class GraphRegistrator
{
public:
	explicit GraphRegistrator(const char* graphType) :
		graphCreator(graphType)
	{
		GraphFactory::instance().addCreator(graphCreator);
	}

	GraphRegistrator(const GraphRegistrator<ConcreteGraph>&) = delete;
	GraphRegistrator<ConcreteGraph>& operator=(const GraphRegistrator<ConcreteGraph>&) = delete;

private:
	const ConcreteGraphCreator<ConcreteGraph> graphCreator;
};

#endif //__GRAPH_REGISTRATOR_HEADER_INCLUDED__