#ifndef __VERTEX_HEADER_INCLUDED__
#define __VERTEX_HEADER_INCLUDED__

#include "String.h"
#include "LinkedList.h"
#include "Distance.h"
#include "Edge.h"

class Vertex
{
	friend class GraphBase;

public:
	bool isMarkedAsVisited() const;
	void markAsVisited();
	void markAsNotVisited();

	void setParent(Vertex* parent);
	Vertex* getParent();

	void setDistance(const Distance& distance);
	const Distance& getDistance() const;

	const String& getIdentifier() const;
	void setIdentifier(String identifier);

private:
	Vertex(String identifier, size_t index);
	Vertex(const Vertex&) = delete;
	Vertex& operator=(const Vertex&) = delete;
	~Vertex() = default;

private:
	String identifier;
	size_t index;
	LinkedList<Edge> edges;
	Vertex* parent;
	Distance distance;
	bool isVisited;
};

#endif //__VERTEX_HEADER_INCLUDED__