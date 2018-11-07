#include "OutgoingEdge.h"
#include "../Vertex/Vertex.h"

OutgoingEdge::OutgoingEdge(Vertex* v, Weight w)
{
	setVertex(v);
	setWeight(w);
}

void OutgoingEdge::setVertex(Vertex* v)
{
	assert(v != nullptr);

	vertex = v;
}

void OutgoingEdge::setWeight(Weight w)
{
	weight = w;
}

const Vertex& OutgoingEdge::getVertex() const
{
	return *vertex;
}

Vertex& OutgoingEdge::getVertex()
{
	return *vertex;
}

OutgoingEdge::Weight OutgoingEdge::getWeight() const
{
	return weight;
}
