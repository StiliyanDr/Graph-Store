#ifndef __SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__
#define __SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "Hash/Hash Function/HashFunction.h"
#include <unordered_map>

class SearchBasedShortestPathAlgorithm : public ShortestPathAlgorithm
{
protected:
	struct MarkableDecoratedVertex : public DecoratedVertex
	{
		MarkableDecoratedVertex() = default;
		explicit MarkableDecoratedVertex(const Graph::Vertex& originalVertex)
		noexcept :
			DecoratedVertex{ originalVertex },
			isVisited(false)
		{
		}

		bool isVisited;
	};

private:
	using Map = std::unordered_map<std::reference_wrapper<const String>,
		                           MarkableDecoratedVertex,
		                           HashFunction<String>>;

protected:
	explicit SearchBasedShortestPathAlgorithm(String id);
	SearchBasedShortestPathAlgorithm(const SearchBasedShortestPathAlgorithm&) = delete;
	SearchBasedShortestPathAlgorithm&
		operator=(const SearchBasedShortestPathAlgorithm&) = delete;

	void decorateVerticesOf(const Graph& g) override;
	void addDecoratedVersionOf(const Graph::Vertex& v) override;
	MarkableDecoratedVertex&
		getDecoratedVersionOf(const Graph::Vertex& v) override;
	virtual void visitVertex(MarkableDecoratedVertex& successor,
							 const MarkableDecoratedVertex& predecessor);
	void cleanUp() override;
	void setTarget(const Graph::Vertex& target);
	void checkIfTarget(const MarkableDecoratedVertex& v);

protected:
	bool foundAShortestPath;

private:
	const Graph::Vertex* target;
	Map decoratedVertices;
};

#endif //__SEARCH_BASED_SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__