#include "DijkstraAlgorithm.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<DijkstraAlgorithm> registrator("dijkstra");

DijkstraAlgorithm::DijkstraAlgorithm(const String& id) :
	ShortestPathAlgorithm(id)
{
}

ShortestPathAlgorithm::Path
DijkstraAlgorithm::findShortestPath(const Graph& graph,
									const Vertex& source,
									const Vertex& target)
{
	initialiseAlgorithm(graph, source);

	DijkstraVertex* v;

	while (!undeterminedEstimateVertices.isEmpty())
	{
		v = undeterminedEstimateVertices.extractOptimal();

		if (v->originalVertex == target)
		{
			break;
		}

		relaxEdgesLeaving(*v, graph);
	}

	Path p = createPathBetween(source, target);
	cleanUpAlgorithmState();

	return p;
}

void DijkstraAlgorithm::initialiseAlgorithm(const Graph& graph, const Vertex& source)
{
	decorateVerticesOf(graph);
	initialiseSource(getDecoratedVersionOf(source));
	gatherDecoratedVerticesWithUndeterminedEstimate();
}

void DijkstraAlgorithm::decorateVerticesOf(const Graph& graph)
{
	decoratedVertices.reserve(graph.getVerticesCount());

	ShortestPathAlgorithm::decorateVerticesOf(graph);
}

void DijkstraAlgorithm::addDecoratedVersionOf(const Vertex& v)
{
	decoratedVertices.emplace(v.getID(), v);
}

DijkstraAlgorithm::DijkstraVertex& DijkstraAlgorithm::getDecoratedVersionOf(const Vertex& v)
{
	return decoratedVertices[v.getID()];
}

void DijkstraAlgorithm::gatherDecoratedVerticesWithUndeterminedEstimate()
{
	HashIterator iterator = decoratedVertices.begin();

	undeterminedEstimateVertices = PriorityQueue(iterator, decoratedVertices.size());
}

void DijkstraAlgorithm::relaxEdgesLeaving(const DijkstraVertex& start, const Graph& graph)
{
	Graph::EdgesConstIterator iterator =
		graph.getConstIteratorOfEdgesLeaving(start.originalVertex);

	forEach(*iterator, [&](const Edge& e)
	{
		DijkstraVertex& end = getDecoratedVersionOf(e.getVertex());

		relaxEdge(start, end, e.getWeight());
	});
}

void DijkstraAlgorithm::relaxEdge(const DijkstraVertex& start, DijkstraVertex& end, unsigned weight)
{
	Distance distanceThroughStart = start.distanceToSource + weight;

	if (distanceThroughStart < end.distanceToSource)
	{
		undeterminedEstimateVertices.optimiseKey(end.handle, distanceThroughStart);
		end.parent = &start;
	}
}

void DijkstraAlgorithm::cleanUpAlgorithmState()
{
	undeterminedEstimateVertices.empty();
	decoratedVertices.clear();
}
