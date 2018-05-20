#ifndef __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__

#include "String.h"

class Graph;
class Vertex;

class ShortestPathAlgorithm
{
public:
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = delete;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = delete;
	virtual ~ShortestPathAlgorithm() = default;
	
	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& destination) = 0;
	const String& getIdentifier() const;

protected:
	ShortestPathAlgorithm(const char* identifier);

	void initialiseVerticesOf(Graph& graph) const;
	virtual void initialiseVertex(Vertex& vertex) const = 0;

private:
	void setIdentifier(const char* identifier);

private:
	String identifier;
};

#endif //__SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__