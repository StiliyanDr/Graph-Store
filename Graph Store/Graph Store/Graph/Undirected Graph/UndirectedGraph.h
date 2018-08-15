#ifndef __UNDIRECTED_GRAPH_HEADER_INCLUDED__
#define __UNDIRECTED_GRAPH_HEADER_INCLUDED__

#include "../GraphBase/GraphBase.h"

class UndirectedGraph : public GraphBase
{
public:
	explicit UndirectedGraph(const String& identifier);
	UndirectedGraph(const UndirectedGraph&) = delete;
	UndirectedGraph& operator=(const UndirectedGraph&) = delete;
	UndirectedGraph(UndirectedGraph&&) = delete;
	UndirectedGraph& operator=(UndirectedGraph&&) = delete;
	virtual ~UndirectedGraph() = default;

	virtual void addEdgeBetweenWithWeight(Vertex& start, Vertex& end, unsigned weight) override;
	virtual void removeEdgeBetween(Vertex& start, Vertex& end) override;

protected:
	virtual void removeEdgesEndingIn(Vertex& vertex) override;
};

#endif //__UNDIRECTED_GRAPH_HEADER_INCLUDED__