#ifndef __GRAPH_HEADER_INCLUDED__
#define __GRAPH_HEADER_INCLUDED__

#include "../../String/String.h"
#include "../../Iterator/Iterator.h"
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"
#include <memory>

class Graph
{
public:
	typedef std::unique_ptr<ConstIterator<const Vertex*>> VerticesConstIterator;
	typedef std::unique_ptr<ConstIterator<Edge>> EdgesConstIterator;

public:
	virtual ~Graph() = default;

	virtual void addVertex(const String& id) = 0;
	virtual void removeVertex(Vertex& v) = 0;
	virtual void addEdge(Vertex& start, Vertex& end, unsigned weight) = 0;
	virtual void removeEdge(Vertex& start, Vertex& end) = 0;

	virtual Vertex& getVertexWithID(const String& id) = 0;
	virtual VerticesConstIterator getIteratorOfVertices() const = 0;
	virtual EdgesConstIterator getIteratorOfEdgesLeaving(const Vertex& v) const = 0;
	virtual unsigned getVerticesCount() const = 0;

	const String& getID() const;
	void setID(String id);

protected:
	Graph(const String& id);
	Graph(const Graph&) = default;
	Graph& operator=(const Graph&) = default;

private:
	String id;
};

#endif //__GRAPH_HEADER_INCLUDED__