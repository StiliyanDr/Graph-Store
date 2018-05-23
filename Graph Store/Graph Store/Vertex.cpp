#include "stdafx.h"
#include "Vertex.h"
#include <utility>
#include <stdexcept>

Vertex::Vertex(String identifier, size_t index) :
	index(index)
{
	setIdentifier(std::move(identifier));
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

void Vertex::setDistance(const Distance& distance)
{
	this->distance = distance;
}

const Distance& Vertex::getDistance() const
{
	return distance;
}

const String& Vertex::getIdentifier() const
{
	return identifier;
}

void Vertex::setIdentifier(String identifier)
{
	if (identifier != String(""))
	{
		this->identifier = std::move(identifier);
	}
	else
	{
		throw std::invalid_argument("Invalid vertex identifier!");
	}
}