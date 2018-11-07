#include "Graph.h"
#include "../Graph Exception/GraphException.h"

Graph::Edge::Edge(Vertex& start, OutgoingEdge& e) :
	start(start), incidentToStartEdge(e)
{
}

const Vertex& Graph::Edge::getStart() const
{
	return start;
}

const Vertex& Graph::Edge::getEnd() const
{
	return incidentToStartEdge.getVertex();
}

Graph::Edge::Weight Graph::Edge::getWeight() const
{
	return incidentToStartEdge.getWeight();
}

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
