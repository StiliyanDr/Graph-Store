#ifndef __UNDIRECTED_GRAPH_HEADER_INCLUDED__
#define __UNDIRECTED_GRAPH_HEADER_INCLUDED__

#include "GraphBase.h"

class UndirectedGraph : public GraphBase
{
public:
	UndirectedGraph(const char* identifier);
	UndirectedGraph(const UndirectedGraph&) = delete;
	UndirectedGraph& operator=(const UndirectedGraph&) = delete;
	virtual ~UndirectedGraph() = default;

	virtual void addEdgeBetweenWithWeight(Vertex& startVertex, Vertex& endVertex, unsigned weight) override;
	virtual void removeEdgeBetween(Vertex& startVertex, const Vertex& endVertex) override;

protected:
	virtual void removeEdgesEndingIn(Vertex& vertex) override;
};

#endif //__UNDIRECTED_GRAPH_HEADER_INCLUDED__