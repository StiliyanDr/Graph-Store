#ifndef __DIRECTED_GRAPH_HEADER_INCLUDED__
#define __DIRECTED_GRAPH_HEADER_INCLUDED__

#include "../GraphBase/GraphBase.h"

class DirectedGraph : public GraphBase
{
public:
	DirectedGraph(const String& identifier);
	DirectedGraph(const DirectedGraph&) = delete;
	DirectedGraph& operator=(const DirectedGraph&) = delete;
	DirectedGraph(DirectedGraph&&) = delete;
	DirectedGraph& operator=(DirectedGraph&&) = delete;
	virtual ~DirectedGraph() = default;

	virtual void addEdgeBetweenWithWeight(Vertex& start, Vertex& end, unsigned weight) override;
	virtual void removeEdgeBetween(Vertex& start, Vertex& end) override;
};

#endif //__DIRECTED_GRAPH_HEADER_INCLUDED__