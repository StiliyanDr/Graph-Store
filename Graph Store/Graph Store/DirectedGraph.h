#ifndef __DIRECTED_GRAPH_HEADER_INCLUDED__
#define __DIRECTED_GRAPH_HEADER_INCLUDED__

#include "GraphBase.h"

class DirectedGraph : public GraphBase
{
public:
	DirectedGraph(const char* identifier);
	DirectedGraph(const DirectedGraph&) = delete;
	DirectedGraph& operator=(const DirectedGraph&) = delete;
	virtual ~DirectedGraph() = default;
};

#endif //__DIRECTED_GRAPH_HEADER_INCLUDED__