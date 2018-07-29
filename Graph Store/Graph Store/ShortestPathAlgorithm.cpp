#include "stdafx.h"
#include "ShortestPathAlgorithm.h"

ShortestPathAlgorithm::ShortestPathAlgorithm(const char* identifier)
{
	setIdentifier(identifier);
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

void ShortestPathAlgorithm::initialiseSource(Vertex& source) const
{
	source.setDistance(0);
}
