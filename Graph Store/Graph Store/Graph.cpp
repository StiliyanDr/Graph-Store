#include "stdafx.h"
#include "Graph.h"
#include <utility>
#include <stdexcept>

Graph::Graph(String identifier)
{
	setIdentifier(std::move(identifier));
}

void Graph::setIdentifier(String identifier)
{
	if (identifier != String(""))
	{
		this->identifier = std::move(identifier);
	}
	else
	{
		throw std::invalid_argument("Invalid graph identifier!");
	}
}

const String& Graph::getIdentifier() const
{
	return identifier;
}
