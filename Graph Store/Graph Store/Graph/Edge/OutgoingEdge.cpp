#include "OutgoingEdge.h"
#include "../Vertex/Vertex.h"

OutgoingEdge::OutgoingEdge(Vertex* v, unsigned weight)
{
	setVertex(v);
	setWeight(weight);
}

void OutgoingEdge::setVertex(Vertex* v)
{
	assert(v != nullptr);

	vertex = v;
}

void OutgoingEdge::setWeight(unsigned weight)
{
	this->weight = weight;
}

const Vertex& OutgoingEdge::getVertex() const
{
	return *vertex;
}

Vertex& OutgoingEdge::getVertex()
{
	return *vertex;
}

unsigned OutgoingEdge::getWeight() const
{
	return weight;
}
