#include "stdafx.h"
#include "ShortestPathAlgorithm.h"
#include "Graph.h"
#include "Vertex.h"
#include "ShortestPathAlgorithmsStore.h"

ShortestPathAlgorithm::ShortestPathAlgorithm(const char* identifier)
{
	setIdentifier(identifier);

	ShortestPathAlgorithmsStore::instance().addAlgorithm(*this);
}

void ShortestPathAlgorithm::setIdentifier(const char* identifier)
{
	assert(identifier != nullptr && *identifier);

	this->identifier = identifier;
}

const String& ShortestPathAlgorithm::getIdentifier() const
{
	return identifier;
}

void ShortestPathAlgorithm::initialiseVerticesOf(Graph& graph) const
{
	std::unique_ptr<Iterator<Vertex*>> vertexIterator = graph.getIteratorOfVertices();

	while (vertexIterator->isValid())
	{
		assert(vertexIterator->getCurrentItem() != nullptr);

		initialiseVertex(*(vertexIterator->getCurrentItem()));
		vertexIterator->advance();
	}
}

void ShortestPathAlgorithm::initialiseVertex(Vertex& vertex) const
{
	vertex.markAsNotVisited();
	vertex.setParent(nullptr);
	vertex.setDistance(Distance::getInfinity());
}
