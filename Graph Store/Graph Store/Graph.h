#ifndef __GRAPH_HEADER_INCLUDED__
#define __GRAPH_HEADER_INCLUDED__

#include "String.h"
#include "Iterator.h"
#include "Vertex.h"
#include "Edge.h"

class Graph
{
public:
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;
	virtual ~Graph() = default;
	
	virtual void addVertex(const char* identifier) = 0;
	virtual void removeVertex(const char* identifier) = 0;
	virtual void addEdgeFromToWithWeight(const char* startVertex, const char* endVertex, unsigned weight) = 0;
	virtual void removeEdgeFromTo(const char* startVertex, const char* endVertex) = 0;

	virtual Iterator<Vertex*> getIteratorOfVertices() = 0;
	virtual Iterator<Edge> getIteratorOfEdgesStartingFrom(Vertex& vertex) = 0;

	const String& getIdentifier() const;

protected:
	Graph(String identifier);

private:
	void setIdentifier(String identifier);

private:
	String identifier;
};

#endif //__GRAPH_HEADER_INCLUDED__