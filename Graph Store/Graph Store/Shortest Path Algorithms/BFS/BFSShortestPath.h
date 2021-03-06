#ifndef __BFS_SHORTEST_PATH_HEADER_INCLUDED__
#define __BFS_SHORTEST_PATH_HEADER_INCLUDED__

#include "../Search Based Shortest Path Algorithm/SearchBasedShortestPathAlgorithm.h"
#include "Fixed Size Queue/FixedSizeQueue.h"

class BFSShortestPath : public SearchBasedShortestPathAlgorithm
{
	using Queue = FixedSizeQueue<const MarkableDecoratedVertex*>;

public:
	explicit BFSShortestPath(String id);
	BFSShortestPath(const BFSShortestPath&) = delete;
	BFSShortestPath& operator=(const BFSShortestPath&) = delete;

private:
	void initialise(const Graph& g,
		            const Graph::Vertex& source,
		            const Graph::Vertex& target) override;
	void execute(const Graph& g,
		         const Graph::Vertex& source,
		         const Graph::Vertex& target) override;
	void cleanUp() override;
	void visitVertex(MarkableDecoratedVertex& successor,
					 const MarkableDecoratedVertex& predecessor) override;
	void initialiseFrontier(std::size_t maxSize);
	void initialiseSourceAndAddItToFrontier(const Graph::Vertex& source);
	void addToFrontier(const MarkableDecoratedVertex& v);
	void prepareTrivialPath(const Graph::Vertex& source);
	const MarkableDecoratedVertex* extractNextVertexFromFrontier();
	void exploreEdgesLeaving(const MarkableDecoratedVertex& v,
		                     const Graph& g);
	void exploreEdge(const MarkableDecoratedVertex& predecessor,
		             MarkableDecoratedVertex& successor);
private:
	Queue frontier;
};

#endif //__BFS_SHORTEST_PATH_HEADER_INCLUDED__