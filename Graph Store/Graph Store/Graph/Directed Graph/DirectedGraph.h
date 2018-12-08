#ifndef __DIRECTED_GRAPH_HEADER_INCLUDED__
#define __DIRECTED_GRAPH_HEADER_INCLUDED__

#include "../Abstract class/Graph.h"

class DirectedGraph : public Graph
{
private:
	class DirectedEdgesConstIterator : public EdgesConstIteratorBase
	{
	public:
		DirectedEdgesConstIterator(const VerticesConcreteConstIterator& verticesIterator,
			                       const OutgoingEdgesConcreteConstIterator& edgesIterator) :
			EdgesConstIteratorBase(verticesIterator, edgesIterator)
		{
		}

	private:
		virtual bool pointsToUniteratedEdge() const override
		{
			return pointsToEdge();
		}
	};

public:
	explicit DirectedGraph(const String& id);

	virtual void addEdge(Vertex& start, Vertex& end, Edge::Weight weight) override;
	virtual void removeEdge(Vertex& start, Vertex& end) override;
	virtual EdgesConstIterator getConstIteratorOfEdges() const override;

protected:
	virtual void removeEdgesEndingIn(Vertex& v) override;
};

#endif //__DIRECTED_GRAPH_HEADER_INCLUDED__