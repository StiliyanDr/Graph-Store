#include "DijkstraShortestPath.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<DijkstraShortestPath> registrator("dijkstra");

DijkstraShortestPath::DijkstraShortestPath(const String& id) :
	ShortestPathAlgorithm(id)
{
}

void DijkstraShortestPath::initialise(const Graph& graph,
									  const Vertex& source,
									  const Vertex& target)
{
	try
	{
		decorateVerticesOf(graph);
		initialiseSource(getDecoratedVersionOf(source));
		gatherDecoratedVerticesWithUndeterminedEstimate();
	}
	catch (std::bad_alloc&)
	{
		cleanUp();
		throw;
	}
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

DijkstraShortestPath::DijkstraVertex&
DijkstraShortestPath::getDecoratedVersionOf(const Vertex& v)
{
	return decoratedVertices[v.getID()];
}

void DijkstraShortestPath::gatherDecoratedVerticesWithUndeterminedEstimate()
{
	HashIterator iterator = decoratedVertices.begin();

	undeterminedEstimateVertices =
		PriorityQueue(iterator, decoratedVertices.size());
}

void DijkstraShortestPath::execute(const Graph& graph,
								   const Vertex& source,
								   const Vertex& target)
{
	DijkstraVertex* v;

	while (!undeterminedEstimateVertices.isEmpty())
	{
		v = undeterminedEstimateVertices.extractOptimal();

		if (v->originalVertex == target)
		{
			return;
		}

		relaxEdgesLeaving(*v, graph);
	}
}

void DijkstraShortestPath::relaxEdgesLeaving(const DijkstraVertex& start,
											 const Graph& graph)
{
	Graph::EdgesConstIterator iterator =
		graph.getConstIteratorOfEdgesLeaving(start.originalVertex);

	forEach(*iterator, [&](const Edge& e)
	{
		DijkstraVertex& end = getDecoratedVersionOf(e.getVertex());

		relaxEdge(start, end, e.getWeight());
	});
}

void DijkstraShortestPath::relaxEdge(const DijkstraVertex& start,
									 DijkstraVertex& end,
									 unsigned weight)
{
	Distance distanceThroughStart = start.distanceToSource + weight;

	if (distanceThroughStart < end.distanceToSource)
	{
		undeterminedEstimateVertices.optimiseKey(end.handle, distanceThroughStart);
		end.parent = &start;
	}
}

void DijkstraShortestPath::cleanUp()
{
	undeterminedEstimateVertices.empty();
	decoratedVertices.clear();
}
