#ifndef __DIRECTED_GRAPH_HEADER_INCLUDED__
#define __DIRECTED_GRAPH_HEADER_INCLUDED__

#include "../Abstract class/Graph.h"

class DirectedGraph : public Graph
{
public:
	explicit DirectedGraph(const String& id);

	virtual void addEdge(Vertex& start, Vertex& end, Edge::Weight weight) override;
	virtual void removeEdge(Vertex& start, Vertex& end) override;

protected:
	virtual void removeEdgesEndingIn(Vertex& v) override;
};

#endif //__DIRECTED_GRAPH_HEADER_INCLUDED__