#ifndef __SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__
#define __SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "../../Hash/Hash Function/HashFunctionStringSpecialization.h"
#include <unordered_map>

class SearchBasedShortestPathAlgorithm : public ShortestPathAlgorithm
{
protected:
	struct MarkableDecoratedVertex : public DecoratedVertex
	{
		MarkableDecoratedVertex(const Vertex& originalVertex) :
			DecoratedVertex(originalVertex),
			isVisited(false)
		{
		}

		bool isVisited;
	};

private:
	typedef std::unordered_map<String, MarkableDecoratedVertex, HashFunction<String>> Hash;

protected:
	SearchBasedShortestPathAlgorithm(const String& id);
	SearchBasedShortestPathAlgorithm(const SearchBasedShortestPathAlgorithm&) = delete;
	SearchBasedShortestPathAlgorithm& operator=(const SearchBasedShortestPathAlgorithm&) = delete;

	virtual DecoratedVertex& getDecoratedVersionOf(const Vertex& vertex) override;
	virtual void visitVertex(MarkableDecoratedVertex& successor,
							 const MarkableDecoratedVertex& predecessor);
	void decorateVerticesOf(const Graph& graph);
	void setTarget(const Vertex& target);
	void checkIfTarget(const MarkableDecoratedVertex& vertex);

private:
	const Vertex* target;
	Hash decoratedVertices;
	bool foundAShortestPath;
};

#endif //__SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__