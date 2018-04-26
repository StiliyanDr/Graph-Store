#include "stdafx.h"
#include "Edge.h"
#include "Vertex.h"
#include <stdexcept>

Edge::Edge(Vertex* vertex, unsigned weight)
{
	setVertex(vertex);
	setWeight(weight);
}

void Edge::setVertex(Vertex* vertex)
{
	if (vertex != nullptr)
		this->vertex = vertex;
	else
		throw std::invalid_argument("Expected a vertex!");
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
