#ifndef __BFS_HEADER_INCLUDED__
#define __BFS_HEADER_INCLUDED__

#include "ShortestPathAlgorithm.h"
#include "Queue.h"

class BFS : public ShortestPathAlgorithm
{
	static BFS theOnlyInstance;

public:
	BFS(const BFS&) = delete;
	BFS& operator=(const BFS&) = delete;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& destination) override;

protected:
	BFS(const char* algorithmIdentifier);
	virtual ~BFS() = default;

	virtual void initialiseVertex(Vertex& vertex) const override;
	virtual void initialiseSource(Vertex& source) const;
	virtual void visitVertex(Vertex& vertex, Vertex& predecessor) const;

private:
	static void buildTrivialPathStartingFrom(Vertex& vertex);

private:
	void initialiseAlgorithm(Graph& graph, Vertex& source, const Vertex& destination);
	void cleanUpAlgorithmState();
	void exploreEdge(Vertex& predecessor, Vertex& successor);
	void checkIfDestination(const Vertex& vertex);
	void addVertexToFrontier(Vertex& vertex);
	Vertex* getNextVertexFromFrontier();
	bool theFrontierIsEmpty() const;
	void setDestination(const Vertex& destination);

private:
	const Vertex* destination;
	Queue<Vertex*> queue;
	bool hasFoundAShortestPath;
};

#endif //__BFS_HEADER_INCLUDED__