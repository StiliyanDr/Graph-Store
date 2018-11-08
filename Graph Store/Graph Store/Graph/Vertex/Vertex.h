#ifndef __VERTEX_HEADER_INCLUDED__
#define __VERTEX_HEADER_INCLUDED__

#include "../../String/String.h"
#include "../../Linked List/LinkedList.h"
#include "../Edge/OutgoingEdge.h"

class Vertex
{
	friend class Graph;

public:
	Vertex(const Vertex&) = delete;
	Vertex& operator=(const Vertex&) = delete;

	const String& getID() const;
	void setID(String id);

private:
	Vertex(String id, size_t index);

private:
	String id;
	size_t index;
	LinkedList<OutgoingEdge> edges;
};

bool operator==(const Vertex& lhs, const Vertex& rhs);
bool operator!=(const Vertex& lhs, const Vertex& rhs);

#endif //__VERTEX_HEADER_INCLUDED__