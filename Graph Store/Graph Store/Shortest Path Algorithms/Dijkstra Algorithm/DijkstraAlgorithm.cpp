#include "DijkstraAlgorithm.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<DijkstraShortestPath> registrator("dijkstra");

DijkstraShortestPath::DijkstraShortestPath(const String& id) :
	ShortestPathAlgorithm(id)
{
}

ShortestPathAlgorithm::Path
DijkstraShortestPath::findShortestPath(const Graph& graph,
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

void DijkstraShortestPath::initialiseAlgorithm(const Graph& graph, const Vertex& source)
{
	decorateVerticesOf(graph);
	initialiseSource(getDecoratedVersionOf(source));
	gatherDecoratedVerticesWithUndeterminedEstimate();
}

void DijkstraShortestPath::decorateVerticesOf(const Graph& graph)
{
	decoratedVertices.reserve(graph.getVerticesCount());

	ShortestPathAlgorithm::decorateVerticesOf(graph);
}

void DijkstraShortestPath::addDecoratedVersionOf(const Vertex& v)
{
	decoratedVertices.emplace(v.getID(), v);
}

DijkstraShortestPath::DijkstraVertex& DijkstraShortestPath::getDecoratedVersionOf(const Vertex& v)
{
	return decoratedVertices[v.getID()];
}

void DijkstraShortestPath::gatherDecoratedVerticesWithUndeterminedEstimate()
{
	HashIterator iterator = decoratedVertices.begin();

	undeterminedEstimateVertices = PriorityQueue(iterator, decoratedVertices.size());
}

void DijkstraShortestPath::relaxEdgesLeaving(const DijkstraVertex& start, const Graph& graph)
{
	Graph::EdgesConstIterator iterator =
		graph.getConstIteratorOfEdgesLeaving(start.originalVertex);

	forEach(*iterator, [&](const Edge& e)
	{
		DijkstraVertex& end = getDecoratedVersionOf(e.getVertex());

		relaxEdge(start, end, e.getWeight());
	});
}

void DijkstraShortestPath::relaxEdge(const DijkstraVertex& start, DijkstraVertex& end, unsigned weight)
{
	Distance distanceThroughStart = start.distanceToSource + weight;

	if (distanceThroughStart < end.distanceToSource)
	{
		undeterminedEstimateVertices.optimiseKey(end.handle, distanceThroughStart);
		end.parent = &start;
	}
}

void DijkstraShortestPath::cleanUpAlgorithmState()
{
	undeterminedEstimateVertices.empty();
	decoratedVertices.clear();
}
