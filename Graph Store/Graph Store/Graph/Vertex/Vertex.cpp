#include "Vertex.h"
#include "../Graph Exception/GraphException.h"

Vertex::Vertex(String id, size_t index) :
	index(index)
{
	setID(std::move(id));
}

void Vertex::setID(String id)
{
	if (id != String(""))
	{
		this->id = std::move(id);
	}
	else
	{
		throw GraphException("Invalid vertex id: " + id);
	}
}

const String& Vertex::getID() const
{
	return id;
}

bool operator!=(const Vertex& lhs, const Vertex& rhs)
{
	return !(lhs == rhs);
}

bool operator==(const Vertex& lhs, const Vertex& rhs)
{
	return &lhs == &rhs;
}
