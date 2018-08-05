#ifndef __DIJKSTRA_ALGORITHM_HEADER_INCLUDED__
#define __DIJKSTRA_ALGORITHM_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "../../Priority Queue/PriorityQueue.h"
#include "../../Priority Queue/Handle Updator/HandleUpdator.h"
#include "../../Priority Queue/Distance Accessor/DistanceAccessor.h"

class DijkstraAlgorithm : public ShortestPathAlgorithm
{
	typedef PriorityQueue<Vertex, Distance, HandleUpdator, DistanceAccessor> PriorityQueue;

public:
	DijkstraAlgorithm(const char* identifier);
	DijkstraAlgorithm(const DijkstraAlgorithm&) = default;
	DijkstraAlgorithm& operator=(const DijkstraAlgorithm&) = default;
	DijkstraAlgorithm(DijkstraAlgorithm&&) = delete;
	DijkstraAlgorithm& operator=(DijkstraAlgorithm&&) = delete;
	virtual ~DijkstraAlgorithm() = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& target) override;

protected:
	virtual void initialiseVertex(Vertex& vertex) const override;

private:
	static PriorityQueue makePriorityQueueContainingVerticesOf(Graph& graph);

private:
	void initialiseAlgorithm(Graph& graph, Vertex& source) const;
	void relaxEdgesLeaving(Vertex& vertex, Graph& graph);
	void relaxEdge(Vertex& startVertex, Vertex& endVertex, unsigned weight);
	void cleanUpAlgorithmState();

private:
	PriorityQueue priorityQueue;
};

#endif //__DIJKSTRA_ALGORITHM_HEADER_INCLUDED__