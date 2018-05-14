#include "stdafx.h"
#include "ShortestPathAlgorithm.h"
#include "Graph.h"
#include "Vertex.h"
#include <stdexcept>

ShortestPathAlgorithm::ShortestPathAlgorithm(const char* identifier)
{
	setIdentifier(identifier);
}

void ShortestPathAlgorithm::setIdentifier(const char* identifier)
{
	if (identifier && *identifier)
	{
		this->identifier = identifier;
	}
	else
	{
		throw std::invalid_argument("Invalid ShortestPathAlgorithm identifier!");
	}
}

const String& ShortestPathAlgorithm::getIdentifier() const
{
	return identifier;
}

void ShortestPathAlgorithm::initialiseGraphAndSource(Graph& graph, Vertex& source) const
{
	std::unique_ptr<Iterator<Vertex*>> vertexIterator = graph.getIteratorOfVertices();

	Vertex* vertex;

	while (vertexIterator->isValid())
	{
		vertex = vertexIterator->getCurrentItem();
		
		vertex->markAsNotVisited();
		vertex->setParent(nullptr);
		vertex->setDistance(Distance::getInfinity());

		vertexIterator->advance();
	}

	source.setDistance(0);
}
