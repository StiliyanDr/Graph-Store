#include "IterativeDeepeningDFS.h"
#include "../Algorithm Registrator/ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<IterativeDeepeningDFS> registrator("dfs-shortest");

IterativeDeepeningDFS::IterativeDeepeningDFS(String id) :
	SearchBasedShortestPathAlgorithm(std::move(id)),
	graph(nullptr)
{
}

void IterativeDeepeningDFS::initialise(const Graph& g,
									   const Graph::Vertex& source,
									   const Graph::Vertex& target)
{
	decorateVerticesOf(g);
	initialiseSource(getDecoratedVersionOf(source));
	setGraph(g);
	setTarget(target);
}

void IterativeDeepeningDFS::execute(const Graph& g,
									const Graph::Vertex& source,
									const Graph::Vertex& target)
{
	auto maxLengthOfShortestPath = g.getVerticesCount() - 1;
	auto depth = 0u;

	while (!foundAShortestPath && depth <= maxLengthOfShortestPath)
	{
		depthLimitedSearch(getDecoratedVersionOf(source), depth);
		++depth;
	}
}

void IterativeDeepeningDFS::depthLimitedSearch(MarkableDecoratedVertex& v,
											   unsigned depth)
{
	v.isVisited = true;

	if (depth == 0)
	{
		checkIfTarget(v);
	}
	else
	{
		expandSearch(v, depth - 1);
	}

	v.isVisited = false;
}

void IterativeDeepeningDFS::expandSearch(const MarkableDecoratedVertex& v,
										 unsigned depth)
{
	auto iterator =
		graph->getConstIteratorOfEdgesLeaving(*(v.originalVertex));
	auto hasFoundAShortestPath =
		[this](const auto&) noexcept { return foundAShortestPath; };

	forEachUntil(*iterator,
		         hasFoundAShortestPath,
				 [this, &v, depth](const auto& edge)
	{
		auto& successor =
			getDecoratedVersionOf(edge.getEnd());

		if (!successor.isVisited)
		{
			visitVertex(successor, v);
			depthLimitedSearch(successor, depth);
		}
	});
}

void IterativeDeepeningDFS::setGraph(const Graph& g) noexcept
{
	graph = &g;
}

void IterativeDeepeningDFS::cleanUp()
{
	graph = nullptr;
	SearchBasedShortestPathAlgorithm::cleanUp();
}
