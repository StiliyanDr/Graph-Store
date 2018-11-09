#include "BFSShortestPath.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"
#include "../../Graph/Abstract class/Graph.h"
#include <assert.h>

static ShortestPathAlgorithmRegistrator<BFSShortestPath> registrator("bfs");

BFSShortestPath::BFSShortestPath(const String& id) :
	SearchBasedShortestPathAlgorithm(id)
{
}

void BFSShortestPath::initialise(const Graph& graph,
								 const Graph::Vertex& source,
								 const Graph::Vertex& target)
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

void BFSShortestPath::initialiseSourceAndAddItToFrontier(const Graph::Vertex& source)
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

void BFSShortestPath::prepareTrivialPath(const Graph::Vertex& source)
{
	addDecoratedVersionOf(source);
	initialiseSource(getDecoratedVersionOf(source));
}

void BFSShortestPath::execute(const Graph& graph,
							  const Graph::Vertex& source,
							  const Graph::Vertex& target)
{
	const MarkableDecoratedVertex* vertex;

	while (!(foundAShortestPath || frontier.isEmpty()))
	{
		vertex = extractNextVertexFromFrontier();
		exploreEdgesLeaving(*vertex, graph);
	}
}

void BFSShortestPath::exploreEdgesLeaving(const MarkableDecoratedVertex& predecessor,
										  const Graph& graph)
{
	Graph::OutgoingEdgesConstIterator iterator =
		graph.getConstIteratorOfEdgesLeaving(*(predecessor.originalVertex));

	forEach(*iterator, [&](const Graph::OutgoingEdge& e)
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

void BFSShortestPath::cleanUp()
{
	frontier.empty();
	SearchBasedShortestPathAlgorithm::cleanUp();
}
