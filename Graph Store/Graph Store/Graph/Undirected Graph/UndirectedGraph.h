#ifndef __UNDIRECTED_GRAPH_HEADER_INCLUDED__
#define __UNDIRECTED_GRAPH_HEADER_INCLUDED__

#include "../Abstract class/Graph.h"

class UndirectedGraph : public Graph
{
public:
	explicit UndirectedGraph(const String& id);

	virtual void addEdge(Vertex& start, Vertex& end, Edge::Weight weight) override;
	virtual void removeEdge(const Vertex& start, const Vertex& end) override;
	virtual EdgesConstIterator getConstIteratorOfEdges() const override;
	virtual unsigned getEdgesCount() const override;
	virtual const String& getType() const override;

private:
	virtual void removeEdgesEndingIn(const Vertex& v) override;
	void tryToAddUndirectedEdge(Vertex& start, Vertex& end, Edge::Weight weight);
	void verifyEdgeWouldNotBeALoop(const Vertex& start, const Vertex& end) const;
};

#endif //__UNDIRECTED_GRAPH_HEADER_INCLUDED__