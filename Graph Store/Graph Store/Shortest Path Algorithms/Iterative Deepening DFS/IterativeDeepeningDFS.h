#ifndef __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__

#include "../Search Based Shortest Path Algorithm/SearchBasedShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public SearchBasedShortestPathAlgorithm
{
public:
	explicit IterativeDeepeningDFS(String id);
	IterativeDeepeningDFS(const IterativeDeepeningDFS&) = delete;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS&) = delete;

private:
	void initialise(const Graph& g,
		            const Graph::Vertex& source,
		            const Graph::Vertex& target) override;
	void execute(const Graph& g,
		         const Graph::Vertex& source,
		         const Graph::Vertex& target) override;
	void cleanUp() override;
	void depthLimitedSearch(MarkableDecoratedVertex& v, unsigned depth);
	void expandSearch(const MarkableDecoratedVertex& v, unsigned depth);
	void setGraph(const Graph& g) noexcept;

private:
	const Graph* graph;
};

#endif //__ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__