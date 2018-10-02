#ifndef __BFS_SHORTEST_PATH_HEADER_INCLUDED__
#define __BFS_SHORTEST_PATH_HEADER_INCLUDED__

#include "../Search Based Shortest Path Algorithm/SearchBasedShortestPathAlgorithm.h"
#include "../../Queue/Queue.h"

class BFSShortestPath : public SearchBasedShortestPathAlgorithm
{
public:
	explicit BFSShortestPath(const String& id);
	BFSShortestPath(const BFSShortestPath&) = delete;
	BFSShortestPath& operator=(const BFSShortestPath&) = delete;

private:
	virtual void initialise(const Graph& graph, const Vertex& source, const Vertex& target) override;
	virtual void execute(const Graph& graph, const Vertex& source, const Vertex& target) override;
	virtual void cleanUp() override;
	virtual void visitVertex(MarkableDecoratedVertex& successor,
							 const MarkableDecoratedVertex& predecessor) override;
	void initialiseSourceAndAddItToFrontier(const Vertex& source);
	void addToFrontier(const MarkableDecoratedVertex& v);
	void prepareTrivialPath(const Vertex& source);
	const MarkableDecoratedVertex* extractNextVertexFromFrontier();
	void exploreEdgesLeaving(const MarkableDecoratedVertex& predecessor, const Graph& graph);
	void exploreEdge(const MarkableDecoratedVertex& predecessor, MarkableDecoratedVertex& successor);

private:
	Queue<const MarkableDecoratedVertex*> frontier;
};

#endif //__BFS_SHORTEST_PATH_HEADER_INCLUDED__