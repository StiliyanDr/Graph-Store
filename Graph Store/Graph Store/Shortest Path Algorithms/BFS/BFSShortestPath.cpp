#include "BFSShortestPath.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"
#include <assert.h>

static ShortestPathAlgorithmRegistrator<BFSShortestPath> registrator("bfs");

BFSShortestPath::BFSShortestPath(String id) :
	SearchBasedShortestPathAlgorithm(std::move(id)),
	frontier(0)
{
}

void BFSShortestPath::initialise(const Graph& graph,
								 const Graph::Vertex& source,
								 const Graph::Vertex& target)
{
	assert(frontier.isEmpty());
	assert(!foundAShortestPath);

	if (source != target)
	{
		setTarget(target);
		decorateVerticesOf(graph);
		initialiseFrontier(graph.getVerticesCount());
		initialiseSourceAndAddItToFrontier(source);
	}
	else
	{
		foundAShortestPath = true;
		prepareTrivialPath(source);
	}
}

void BFSShortestPath::initialiseFrontier(std::size_t maxSize)
{
	frontier = Queue(maxSize);
}

void BFSShortestPath::initialiseSourceAndAddItToFrontier(const Graph::Vertex& source)
{
	auto& decoratedSource =
		getDecoratedVersionOf(source);

	decoratedSource.isVisited = true;
	initialiseSource(decoratedSource);
	addToFrontier(decoratedSource);
}

void BFSShortestPath::addToFrontier(const MarkableDecoratedVertex& v)
{
	frontier.enqueue(&v);
}

void BFSShortestPath::prepareTrivialPath(const Graph::Vertex& source)
{
	addDecoratedVersionOf(source);
	initialiseSource(getDecoratedVersionOf(source));
}

void BFSShortestPath::execute(const Graph& g,
							  const Graph::Vertex& source,
							  const Graph::Vertex& target)
{
	while (!(foundAShortestPath || frontier.isEmpty()))
	{
		auto v = extractNextVertexFromFrontier();
		exploreEdgesLeaving(*v, g);
	}
}

void BFSShortestPath::exploreEdgesLeaving(const MarkableDecoratedVertex& v,
										  const Graph& g)
{
	auto iterator =
		g.getConstIteratorOfEdgesLeaving(*(v.originalVertex));

	forEach(*iterator, [this, &v](const auto& edge)
	{
		auto& successor =
			getDecoratedVersionOf(edge.getEnd());
		exploreEdge(v, successor);
	});
}

void BFSShortestPath::exploreEdge(const MarkableDecoratedVertex& predecessor,
								  MarkableDecoratedVertex& successor)
{
	if (successor.isVisited)
	{
		return;
	}

	visitVertex(successor, predecessor);
	addToFrontier(successor);
	checkIfTarget(successor);
}

void BFSShortestPath::visitVertex(MarkableDecoratedVertex& v,
								  const MarkableDecoratedVertex& predecessor)
{
	assert(!v.isVisited);
	v.isVisited = true;

	SearchBasedShortestPathAlgorithm::visitVertex(v, predecessor);
}

const BFSShortestPath::MarkableDecoratedVertex*
BFSShortestPath::extractNextVertexFromFrontier()
{
	return frontier.dequeue();
}

void BFSShortestPath::cleanUp()
{
	frontier.empty();
	SearchBasedShortestPathAlgorithm::cleanUp();
}
