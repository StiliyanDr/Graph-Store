#ifndef __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public ShortestPathAlgorithm
{
public:
	IterativeDeepeningDFS(const char* identifier);
	IterativeDeepeningDFS(const IterativeDeepeningDFS&) = default;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS&) = default;
	IterativeDeepeningDFS(IterativeDeepeningDFS&&) = delete;
	IterativeDeepeningDFS& operator=(IterativeDeepeningDFS&&) = delete;
	virtual ~IterativeDeepeningDFS() = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& target) override;

protected:
	virtual void initialiseVertex(Vertex& vertex) const override;
	virtual void visitVertex(Vertex& vertex, Vertex& predecessor) const;

private:
	void initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& target);
	void depthLimitedSearch(Vertex& vertex, unsigned depth);
	void expandSearch(Vertex& vertex, unsigned depth);
	void checkIfTarget(const Vertex& vertex);
	void setTarget(const Vertex& target);
	void setGraph(Graph& graph);

private:
	Graph* graph;
	const Vertex* target;
	bool hasFoundAShortestPath;
};

#endif //__ITERATIVE_DEEPENING_DFS_HEADER_INCLUDED__