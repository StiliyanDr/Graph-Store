#ifndef __CONCRETE_GRAPH_CREATOR_HEADER_INCLUDED__
#define __CONCRETE_GRAPH_CREATOR_HEADER_INCLUDED__

#include "GraphCreator.h"

template <class ConcreteGraph>
class ConcreteGraphCreator : public GraphCreator
{
public:
	ConcreteGraphCreator(const char* criterion);
	ConcreteGraphCreator(const ConcreteGraphCreator<ConcreteGraph>&) = default;
	ConcreteGraphCreator<ConcreteGraph>& operator=(const ConcreteGraphCreator<ConcreteGraph>&) = delete;
	virtual ~ConcreteGraphCreator() = default;

	virtual std::unique_ptr<Graph> createGraph(const char* identifier) const override;
};

#include "ConcreteGraphCreator.hpp"

#endif //__CONCRETE_GRAPH_CREATOR_HEADER_INCLUDED__