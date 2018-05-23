#ifndef __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__

#include "ShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public ShortestPathAlgorithm
{
	static IterativeDeepeningDFS theOnlyInstance;

public:
	IterativeDeepeningDFS(const IterativeDeepeningDFS&) = delete;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS&) = delete;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& destination) override;

protected:
	IterativeDeepeningDFS(const char* identifier);
	virtual ~IterativeDeepeningDFS() = default;

	virtual void initialiseVertex(Vertex& vertex) const override;
	virtual void initialiseSource(Vertex& source) const;
	virtual void visitVertex(Vertex& vertex, Vertex& predecessor) const;

private:
	void initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& destination);
	void depthLimitedSearch(Vertex& vertex, unsigned depth);
	void expandSearch(Vertex& vertex, unsigned depth);
	void checkIfDestination(const Vertex& vertex);
	void setDestination(const Vertex& destination);
	void setGraph(Graph& graph);

private:
	Graph* graph;
	const Vertex* destination;
	bool hasFoundAShortestPath;
};

#endif //__ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__