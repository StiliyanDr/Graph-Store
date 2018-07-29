#include "stdafx.h"
#include "DijkstraAlgorithm.h"

DijkstraAlgorithm::DijkstraAlgorithm(const char* identifier) :
	ShortestPathAlgorithm(identifier)
{
}

void DijkstraAlgorithm::findShortestPath(Graph& graph, Vertex& source, Vertex& target)
{
	initialiseAlgorithm(graph, source);

	priorityQueue = makePriorityQueueContainingVerticesOf(graph);
	Vertex* vertex;

	while (!priorityQueue.isEmpty())
	{
		vertex = priorityQueue.extractMin();

		if (*vertex == target)
		{
			break;
		}

		relaxEdgesLeaving(*vertex, graph);
	}

	cleanUpAlgorithmState();
}

void DijkstraAlgorithm::initialiseAlgorithm(Graph& graph, Vertex& source) const
{
	initialiseVerticesOf(graph);
	initialiseSource(source);
}

void DijkstraAlgorithm::initialiseVertex(Vertex& vertex) const
{
	ShortestPathAlgorithm::initialiseVertex(vertex);
}

void DijkstraAlgorithm::relaxEdgesLeaving(Vertex& vertex, Graph& graph)
{
	std::unique_ptr<Iterator<Edge>> edgesIterator =
		graph.getIteratorOfEdgesStartingFrom(vertex);

	while (edgesIterator->isValid())
	{
		Edge& edge = edgesIterator->getCurrentItem();
		relaxEdge(vertex, edge.getVertex(), edge.getWeight());
		edgesIterator->advance();
	}
}

void DijkstraAlgorithm::relaxEdge(Vertex& startVertex, Vertex& endVertex, unsigned weight)
{
	Distance distanceThroughStartVertex = startVertex.getDistance() + weight;

	if (distanceThroughStartVertex < endVertex.getDistance())
	{
		PriorityQueueHandle handle = endVertex.getPriorityQueueHandle();
		priorityQueue.decreaseKey(handle, distanceThroughStartVertex);

		endVertex.setParent(&startVertex);
	}
}

DijkstraAlgorithm::PriorityQueue
DijkstraAlgorithm::makePriorityQueueContainingVerticesOf(Graph& graph)
{
	std::unique_ptr<Iterator<Vertex*>> iterator = graph.getIteratorOfVertices();

	return PriorityQueue(*iterator, graph.getVerticesCount());
}

void DijkstraAlgorithm::cleanUpAlgorithmState()
{
	priorityQueue.empty();
}
