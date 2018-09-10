#include "BFSShortestPath.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"
#include <assert.h>

static ShortestPathAlgorithmRegistrator<BFSShortestPath> registrator("bfs");

BFSShortestPath::BFSShortestPath(const String& id) :
	SearchBasedShortestPathAlgorithm(id)
{
}

ShortestPathAlgorithm::Path
BFSShortestPath::findShortestPath(const Graph& graph,
								  const Vertex& source,
								  const Vertex& target)
{
	initialiseAlgorithm(graph, source, target);

	const MarkableDecoratedVertex* vertex;

	while (!(foundAShortestPath || frontier.isEmpty()))
	{
		vertex = extractNextVertexFromFrontier();
		exploreEdgesLeaving(*vertex, graph);
	}

	Path result = createPathBetween(source, target);

	cleanUpAlgorithmState();

	return result;
}

void BFSShortestPath::initialiseAlgorithm(const Graph& graph,
										  const Vertex& source,
										  const Vertex& target)
{
	assert(frontier.isEmpty());

	if (source != target)
	{
		foundAShortestPath = false;
		setTarget(target);
		decorateVerticesOf(graph);
		initialiseSourceAndAddItToFrontier(source);
	}
	else
	{
		foundAShortestPath = true;
		prepareTrivialPath(source);
	}
}

void BFSShortestPath::initialiseSourceAndAddItToFrontier(const Vertex& source)
{
	MarkableDecoratedVertex& decoratedSource = getDecoratedVersionOf(source);

	decoratedSource.isVisited = true;
	initialiseSource(decoratedSource);
	addToFrontier(decoratedSource);
}

void BFSShortestPath::addToFrontier(const MarkableDecoratedVertex& v)
{
	frontier.enqueue(&v);
}

void BFSShortestPath::prepareTrivialPath(const Vertex& source)
{
	addDecoratedVersionOf(source);
	initialiseSource(getDecoratedVersionOf(source));
}

void BFSShortestPath::exploreEdgesLeaving(const MarkableDecoratedVertex& predecessor,
	const Graph& graph)
{
	Graph::EdgesConstIterator iterator =
		graph.getConstIteratorOfEdgesLeaving(predecessor.originalVertex);

	forEach(*iterator, [&](const Edge& e)
	{
		MarkableDecoratedVertex& successor = getDecoratedVersionOf(e.getVertex());

		exploreEdge(predecessor, successor);
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

void BFSShortestPath::visitVertex(MarkableDecoratedVertex& successor,
								  const MarkableDecoratedVertex& predecessor)
{
	assert(!successor.isVisited);
	successor.isVisited = true;

	SearchBasedShortestPathAlgorithm::visitVertex(successor, predecessor);
}

const BFSShortestPath::MarkableDecoratedVertex*
BFSShortestPath::extractNextVertexFromFrontier()
{
	return frontier.dequeue();
}

void BFSShortestPath::cleanUpAlgorithmState()
{
	frontier.empty();
	removeDecoratedVertices();
}
