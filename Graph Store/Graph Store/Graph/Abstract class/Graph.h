#ifndef __GRAPH_HEADER_INCLUDED__
#define __GRAPH_HEADER_INCLUDED__

#include "../../String/String.h"
#include "../../Iterator/Iterator.h"
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"
#include <memory>

class Graph
{
protected:
	typedef std::unique_ptr<Iterator<Vertex*>> VertexAbstractIterator;
	typedef std::unique_ptr<Iterator<Edge>> EdgeAbstractIterator;

public:
	Graph(Graph&&) = delete;
	Graph& operator=(Graph&&) = delete;
	virtual ~Graph() = default;

	virtual void addVertex(const String& id) = 0;
	virtual void removeVertex(Vertex& vertex) = 0;
	virtual void addEdgeBetweenWithWeight(Vertex& start, Vertex& end, unsigned weight) = 0;
	virtual void removeEdgeBetween(Vertex& start, Vertex& end) = 0;

	virtual Vertex& getVertexWithID(const String& id) = 0;
	virtual VertexAbstractIterator getIteratorOfVertices() = 0;
	virtual EdgeAbstractIterator getIteratorOfEdgesStartingFrom(Vertex& vertex) = 0;
	virtual unsigned getVerticesCount() const = 0;

	const String& getID() const;
	void setID(String id);

protected:
	Graph(String id);
	Graph(const Graph&) = default;
	Graph& operator=(const Graph&) = default;

private:
	String id;
};

#endif //__GRAPH_HEADER_INCLUDED__