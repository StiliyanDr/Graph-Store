#ifndef __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__

#include "../../String/String.h"

class Graph;
class Vertex;

class ShortestPathAlgorithm
{
public:
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = default;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = default;
	virtual ~ShortestPathAlgorithm() = default;
	
	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& target) = 0;
	const String& getIdentifier() const;
	void setIdentifier(const char* identifier);

protected:
	ShortestPathAlgorithm(const char* identifier);

	void initialiseVerticesOf(Graph& graph) const;
	virtual void initialiseVertex(Vertex& vertex) const = 0;
	virtual void initialiseSource(Vertex& source) const;

private:
	String identifier;
};

#endif //__SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__