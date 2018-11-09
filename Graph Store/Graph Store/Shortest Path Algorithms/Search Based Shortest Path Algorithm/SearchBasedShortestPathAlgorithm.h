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
		MarkableDecoratedVertex() = default;
		MarkableDecoratedVertex(const Graph::Vertex& originalVertex) :
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

	virtual void cleanUp() override;
	virtual void addDecoratedVersionOf(const Graph::Vertex& v) override;
	virtual MarkableDecoratedVertex& getDecoratedVersionOf(const Graph::Vertex& v) override;
	virtual void visitVertex(MarkableDecoratedVertex& successor,
							 const MarkableDecoratedVertex& predecessor);
	void decorateVerticesOf(const Graph& g);
	void setTarget(const Graph::Vertex& target);
	void checkIfTarget(const MarkableDecoratedVertex& v);

protected:
	bool foundAShortestPath;

private:
	const Graph::Vertex* target;
	Hash decoratedVertices;
};

#endif //__SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__