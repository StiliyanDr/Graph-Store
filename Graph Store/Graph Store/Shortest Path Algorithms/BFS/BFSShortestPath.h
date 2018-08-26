#ifndef __BFS_HEADER_INCLUDED__
#define __BFS_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "../../Queue/Queue.h"

class BFSShortestPath : public ShortestPathAlgorithm
{
public:
	explicit BFSShortestPath(const char* id);
	BFSShortestPath(const BFSShortestPath&) = default;
	BFSShortestPath& operator=(const BFSShortestPath&) = default;
	BFSShortestPath(BFSShortestPath&&) = delete;
	BFSShortestPath& operator=(BFSShortestPath&&) = delete;
	virtual ~BFSShortestPath() = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& target) override;

protected:
	virtual void initialiseVertex(Vertex& vertex) const override;
	virtual void initialiseSource(Vertex& source) const override;
	virtual void visitVertex(Vertex& vertex, Vertex& predecessor) const;

private:
	static void buildTrivialPathStartingFrom(Vertex& vertex);

private:
	void initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& target);
	void cleanUpAlgorithmState();
	void exploreEdge(Vertex& predecessor, Vertex& successor);
	void checkIfTarget(const Vertex& vertex);
	void addVertexToFrontier(Vertex& vertex);
	Vertex* getNextVertexFromFrontier();
	bool theFrontierIsEmpty() const;
	void setTarget(const Vertex& target);

private:
	const Vertex* target;
	Queue<Vertex*> queue;
	bool hasFoundAShortestPath;
};

#endif //__BFS_HEADER_INCLUDED__