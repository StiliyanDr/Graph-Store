#ifndef __GRAPH_HEADER_INCLUDED__
#define __GRAPH_HEADER_INCLUDED__

#include "String.h"
#include "Iterator.h"
#include "Vertex.h"
#include "Edge.h"
#include <memory>

class Graph
{
protected:
	typedef std::unique_ptr<Iterator<Vertex*>> VertexAbstractIterator;
	typedef std::unique_ptr<Iterator<Edge>> EdgeAbstractIterator;

public:
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;
	virtual ~Graph() = default;

	virtual void addVertex(const char* identifier) = 0;
	virtual void removeVertex(Vertex& vertexToRemove) = 0;
	virtual void addEdgeBetweenWithWeight(Vertex& startVertex, Vertex& endVertex, unsigned weight) = 0;
	virtual void removeEdgeBetween(Vertex& startVertex, Vertex& endVertex) = 0;

	virtual Vertex& getVertexWithIdentifier(const char* identifier) = 0;
	virtual VertexAbstractIterator getIteratorOfVertices() = 0;
	virtual EdgeAbstractIterator getIteratorOfEdgesStartingFrom(Vertex& vertex) = 0;
	virtual unsigned getVerticesCount() const = 0;

	const String& getIdentifier() const;

protected:
	Graph(String identifier);

private:
	void setIdentifier(String identifier);

private:
	String identifier;
};

#endif //__GRAPH_HEADER_INCLUDED__