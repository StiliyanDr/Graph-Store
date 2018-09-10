#include "DijkstraAlgorithm.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<DijkstraAlgorithm> registrator("dijkstra");

DijkstraAlgorithm::DijkstraAlgorithm(const char* id) :
	ShortestPathAlgorithm(id)
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
	Graph::EdgesConstIterator iterator =
		graph.getConstIteratorOfEdgesLeaving(vertex);

	forEach(*iterator, [&](Edge& edge)
	{
		relaxEdge(vertex, edge.getVertex(), edge.getWeight());
	});
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
	Graph::VerticesConstIterator iterator = graph.getConstIteratorOfVertices();

	return PriorityQueue(*iterator, graph.getVerticesCount());
}

void DijkstraAlgorithm::cleanUpAlgorithmState()
{
	priorityQueue.empty();
}
