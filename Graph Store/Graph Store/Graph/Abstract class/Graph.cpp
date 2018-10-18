#include "Graph.h"
#include "../Graph Exception/GraphException.h"

Graph::Graph(const String& id)
{
	setID(id);
}

void Graph::setID(String id)
{
	if (id != ""_s)
	{
		this->id = std::move(id);
	}
	else
	{
		throw GraphException("Invalid graph id: " + id);
	}
}

const String& Graph::getID() const
{
	return id;
}
