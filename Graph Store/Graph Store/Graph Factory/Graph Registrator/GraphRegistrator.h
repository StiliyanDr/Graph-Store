#ifndef __GRAPH_REGISTRATOR_HEADER_INCLUDED__
#define __GRAPH_REGISTRATOR_HEADER_INCLUDED__

#include "../Graph Creator/ConcreteGraphCreator.h"

template <class ConcreteGraph>
class GraphRegistrator
{
public:
	GraphRegistrator(const char* graphType);
	GraphRegistrator(const GraphRegistrator<ConcreteGraph>&) = delete;
	GraphRegistrator<ConcreteGraph>& operator=(const GraphRegistrator<ConcreteGraph>&) = delete;
	GraphRegistrator(GraphRegistrator<ConcreteGraph>&&) = delete;
	GraphRegistrator<ConcreteGraph>& operator=(GraphRegistrator<ConcreteGraph>&&) = delete;
	~GraphRegistrator() = default;

private:
	const ConcreteGraphCreator<ConcreteGraph> graphCreator;
};

#include "GraphRegistrator.hpp"

#endif //__GRAPH_REGISTRATOR_HEADER_INCLUDED__