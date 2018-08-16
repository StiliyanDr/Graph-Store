#include "Vertex.h"
#include "../Graph Exception/GraphException.h"

Vertex::Vertex(String id, size_t index) :
	index(index)
{
	setID(std::move(id));
	setParent(nullptr);
	setDistance(Distance::getInfinity());
	markAsNotVisited();
}

bool Vertex::isMarkedAsVisited() const
{
	return isVisited;
}

void Vertex::markAsVisited()
{
	isVisited = true;
}

void Vertex::markAsNotVisited()
{
	isVisited = false;
}

void Vertex::setParent(Vertex* parent)
{
	this->parent = parent;
}

Vertex* Vertex::getParent()
{
	return parent;
}

const Vertex* Vertex::getParent() const
{
	return parent;
}

void Vertex::setDistance(const Distance& distance)
{
	this->distance = distance;
}

const Distance& Vertex::getDistance() const
{
	return distance;
}

void Vertex::setPriorityQueueHandle(const PriorityQueueHandle& handle)
{
	this->handle = handle;
}

const PriorityQueueHandle& Vertex::getPriorityQueueHandle() const
{
	return handle;
}

const String& Vertex::getID() const
{
	return id;
}

void Vertex::setID(String id)
{
	if (id != String(""))
	{
		this->id = std::move(id);
	}
	else
	{
		throw GraphException("Invalid vertex identifier: " + id);
	}
}

bool operator!=(const Vertex& lhs, const Vertex& rhs)
{
	return !(lhs == rhs);
}

bool operator==(const Vertex& lhs, const Vertex& rhs)
{
	return &lhs == &rhs;
}
