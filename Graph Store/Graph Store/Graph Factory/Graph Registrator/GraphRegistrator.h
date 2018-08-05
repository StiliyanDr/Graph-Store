#ifndef __GRAPH_REGISTRATOR_HEADER_INCLUDED__
#define __GRAPH_REGISTRATOR_HEADER_INCLUDED__

#include "../Graph Creator/ConcreteGraphCreator.h"
#include "../GraphFactory.h"

template <class ConcreteGraph>
class GraphRegistrator
{
public:
	GraphRegistrator(const char* graphType) :
		graphCreator(graphType)
	{
		GraphFactory::addCreator(graphCreator);
	}

private:
	const ConcreteGraphCreator<ConcreteGraph> graphCreator;
};

#endif //__GRAPH_REGISTRATOR_HEADER_INCLUDED__