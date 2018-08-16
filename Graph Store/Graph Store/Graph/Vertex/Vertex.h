#ifndef __VERTEX_HEADER_INCLUDED__
#define __VERTEX_HEADER_INCLUDED__

#include "../../String/String.h"
#include "../../Linked List/LinkedList.h"
#include "../../Distance/Distance.h"
#include "../Edge/Edge.h"
#include "../../Priority Queue/Handle/PriorityQueueHandle.h"

class Vertex
{
	friend class GraphBase;

public:
	Vertex(const Vertex&) = delete;
	Vertex& operator=(const Vertex&) = delete;
	Vertex(Vertex&&) = delete;
	Vertex& operator=(Vertex&&) = delete;
	~Vertex() = default;

	bool isMarkedAsVisited() const;
	void markAsVisited();
	void markAsNotVisited();

	void setParent(Vertex* parent);
	Vertex* getParent();
	const Vertex* getParent() const;

	void setDistance(const Distance& distance);
	const Distance& getDistance() const;

	void setPriorityQueueHandle(const PriorityQueueHandle& handle);
	const PriorityQueueHandle& getPriorityQueueHandle() const;

	const String& getID() const;

private:
	Vertex(String id, size_t index);
	
	void setID(String id);

private:
	String id;
	size_t index;
	LinkedList<Edge> edges;
	Vertex* parent;
	PriorityQueueHandle handle;
	Distance distance;
	bool isVisited;
};

bool operator==(const Vertex& lhs, const Vertex& rhs);
bool operator!=(const Vertex& lhs, const Vertex& rhs);

#endif //__VERTEX_HEADER_INCLUDED__