#include "ShortestPathAlgorithm.h"
#include <assert.h>
#include "../../Graph/Abstract class/Graph.h"
#include "../../Graph/Vertex/Vertex.h"

ShortestPathAlgorithm::ShortestPathAlgorithm(const char* id)
{
	setID(id);
}

void ShortestPathAlgorithm::setID(const char* id)
{
	assert(id != nullptr && *id);

	this->id = id;
}

const String& ShortestPathAlgorithm::getID() const
{
	return id;
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
