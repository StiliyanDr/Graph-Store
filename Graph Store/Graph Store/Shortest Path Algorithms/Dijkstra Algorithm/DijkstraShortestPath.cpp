#include "DijkstraShortestPath.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<DijkstraShortestPath> registrator("dijkstra");

DijkstraShortestPath::DijkstraShortestPath(String id) :
	ShortestPathAlgorithm(std::move(id))
{
}

void DijkstraShortestPath::initialise(const Graph& g,
									  const Graph::Vertex& source,
									  const Graph::Vertex& target)
{
	decorateVerticesOf(g);
	initialiseSource(getDecoratedVersionOf(source));
	gatherDecoratedVerticesWithUndeterminedEstimate();
}

void DijkstraShortestPath::decorateVerticesOf(const Graph& g)
{
	decoratedVertices.reserve(g.getVerticesCount());

	ShortestPathAlgorithm::decorateVerticesOf(g);
}

void DijkstraShortestPath::addDecoratedVersionOf(const Graph::Vertex& v)
{
	decoratedVertices.emplace(v.getID(), v);
}

DijkstraShortestPath::DijkstraVertex&
DijkstraShortestPath::getDecoratedVersionOf(const Graph::Vertex& v)
{
	return decoratedVertices.at(v.getID());
}

void DijkstraShortestPath::gatherDecoratedVerticesWithUndeterminedEstimate()
{
	auto begin = MapIterator{ decoratedVertices.begin() };
	auto end = MapIterator{ decoratedVertices.end() };

	undeterminedEstimateVertices = PriorityQueue(begin, end);
}

void DijkstraShortestPath::execute(const Graph& g,
								   const Graph::Vertex& source,
								   const Graph::Vertex& target)
{
	while (!undeterminedEstimateVertices.isEmpty())
	{
		auto v =
			undeterminedEstimateVertices.extractOptimal();

		if (*(v->originalVertex) == target)
		{
			return;
		}

		relaxEdgesLeaving(*v, g);
	}
}

void DijkstraShortestPath::relaxEdgesLeaving(const DijkstraVertex& v,
											 const Graph& g)
{
	auto iterator =
		g.getConstIteratorOfEdgesLeaving(*(v.originalVertex));

	forEach(*iterator, [this, &v](const auto& edge)
	{
		auto& end =
			getDecoratedVersionOf(edge.getEnd());

		relaxEdge(v, end, edge.getWeight());
	});
}

void DijkstraShortestPath::relaxEdge(const DijkstraVertex& start,
									 DijkstraVertex& end,
									 Graph::OutgoingEdge::Weight weight)
{
	auto distanceThroughStart = start.distanceToSource + weight;

	if (distanceThroughStart < end.distanceToSource)
	{
		undeterminedEstimateVertices.optimiseKey(end.handle,
			                                     distanceThroughStart);
		end.parent = &start;
	}
}

void DijkstraShortestPath::cleanUp()
{
	undeterminedEstimateVertices.empty();
	decoratedVertices.clear();
}
