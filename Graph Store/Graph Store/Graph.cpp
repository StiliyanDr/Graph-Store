#include "stdafx.h"
#include "Graph.h"
#include "GraphException.h"

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
		throw GraphException("Invalid graph identifier: " + identifier);
	}
}

const String& Graph::getIdentifier() const
{
	return identifier;
}
