#ifndef __DIRECTED_GRAPH_HEADER_INCLUDED__
#define __DIRECTED_GRAPH_HEADER_INCLUDED__

#include "../GraphBase/GraphBase.h"
#include "../../Graph Factory/Graph Registrator/GraphRegistrator.h"

class DirectedGraph : public GraphBase
{
public:
	DirectedGraph(const String& identifier);
	DirectedGraph(const DirectedGraph&) = delete;
	DirectedGraph& operator=(const DirectedGraph&) = delete;
	virtual ~DirectedGraph() = default;

	virtual void addEdgeBetweenWithWeight(Vertex& startVertex, Vertex& endVertex, unsigned weight) override;
	virtual void removeEdgeBetween(Vertex& startVertex, Vertex& endVertex) override;

private:
	static GraphRegistrator<DirectedGraph> registrator;
};

#endif //__DIRECTED_GRAPH_HEADER_INCLUDED__