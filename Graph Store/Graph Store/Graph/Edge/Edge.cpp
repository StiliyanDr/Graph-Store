#include "Edge.h"
#include "../Vertex/Vertex.h"

Edge::Edge(Vertex* v, unsigned weight)
{
	setVertex(v);
	setWeight(weight);
}

void Edge::setVertex(Vertex* v)
{
	assert(v != nullptr);

	vertex = v;
}

void Edge::setWeight(unsigned weight)
{
	this->weight = weight;
}

const Vertex& Edge::getVertex() const
{
	return *vertex;
}

Vertex& Edge::getVertex()
{
	return *vertex;
}

unsigned Edge::getWeight() const
{
	return weight;
}
