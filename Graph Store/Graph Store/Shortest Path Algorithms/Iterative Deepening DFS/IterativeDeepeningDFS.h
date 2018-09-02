#ifndef __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__

#include "../Search Based Shortest Path Algorithm/SearchBasedShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public SearchBasedShortestPathAlgorithm
{
public:
	explicit IterativeDeepeningDFS(const String& id);
	IterativeDeepeningDFS(const IterativeDeepeningDFS&) = delete;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS&) = delete;

	virtual Path findShortestPath(const Graph& graph,
								  const Vertex& source,
								  const Vertex& target) override;
private:
	void initialiseAlgorithm(const Graph& graph, const Vertex& source, const Vertex& target);
	void depthLimitedSearch(MarkableDecoratedVertex& vertex, unsigned depth);
	void expandSearch(const MarkableDecoratedVertex& predecessor, unsigned depth);
	void cleanUpAlgorithmState();
	void setGraph(const Graph& graph);

private:
	const Graph* graph;
};

#endif //__ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__