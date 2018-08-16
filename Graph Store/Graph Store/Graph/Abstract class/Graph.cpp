#include "Graph.h"
#include "../Graph Exception/GraphException.h"

Graph::Graph(String id)
{
	setID(std::move(id));
}

void Graph::setID(String id)
{
	if (id != String(""))
	{
		this->id = std::move(id);
	}
	else
	{
		throw GraphException("Invalid graph identifier: " + id);
	}
}

const String& Graph::getID() const
{
	return id;
}
