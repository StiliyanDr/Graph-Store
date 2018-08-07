#include "Edge.h"
#include "../Vertex/Vertex.h"

Edge::Edge(Vertex* vertex, unsigned weight)
{
	setVertex(vertex);
	setWeight(weight);
}

void Edge::setVertex(Vertex* vertex)
{
	assert(vertex != nullptr);

	this->vertex = vertex;
}

void Edge::setWeight(unsigned weight)
{
	this->weight = weight;
}

Vertex& Edge::getVertex()
{
	return *vertex;
}

unsigned Edge::getWeight() const
{
	return weight;
}
