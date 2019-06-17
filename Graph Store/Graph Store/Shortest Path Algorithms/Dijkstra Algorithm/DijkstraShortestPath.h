#ifndef __DIJKSTRA_SHORTEST_PATH_HEADER_INCLUDED__
#define __DIJKSTRA_SHORTEST_PATH_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "Priority Queue/PriorityQueue.h"
#include "Hash/Hash Function/HashFunction.h"
#include "Graph/Abstract class/Graph.h"
#include <unordered_map>

class DijkstraShortestPath : public ShortestPathAlgorithm
{
	struct DijkstraVertex : public DecoratedVertex
	{
		DijkstraVertex() = default;
		DijkstraVertex(const Graph::Vertex& originalVertex) noexcept :
			DecoratedVertex(originalVertex)
		{
		}

		PriorityQueueHandle handle;
	};

	class KeyAccessor
	{
	public:
		const Distance& getKeyOf(const DijkstraVertex* v)
		const noexcept
		{
			return v->distanceToSource;
		}

		void setKeyOfWith(DijkstraVertex* v, const Distance& d)
		const noexcept
		{
			v->distanceToSource = d;
		}
	};

	class HandleUpdator
	{
	public:
		void operator()(DijkstraVertex* v, const PriorityQueueHandle& h)
		const noexcept
		{
			v->handle = h;
		}
	};

	using PriorityQueue = PriorityQueue<DijkstraVertex*, Less, Distance, KeyAccessor, HandleUpdator>;
	using Hash = std::unordered_map<std::reference_wrapper<const String>, DijkstraVertex, HashFunction<String>>;

	class HashIterator
	{
	public:
		using difference_type = Hash::iterator::difference_type;
		using value_type = Hash::iterator::value_type*;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = Hash::iterator::iterator_category;

		HashIterator(const Hash::iterator& iterator) :
			iterator(iterator)
		{
		}

		HashIterator& operator++()
		{
			++iterator;

			return *this;
		}

		DijkstraVertex* operator*() const
		{
			return &(iterator->second);
		}

		friend bool operator!=(const HashIterator& lhs,
			                   const HashIterator& rhs)
		{
			return lhs.iterator != rhs.iterator;
		}

	private:
		Hash::iterator iterator;
	};

public:
	explicit DijkstraShortestPath(const String& id);
	DijkstraShortestPath(const DijkstraShortestPath&) = delete;
	DijkstraShortestPath& operator=(const DijkstraShortestPath&) = delete;

private:
	virtual void initialise(const Graph& g, const Graph::Vertex& source, const Graph::Vertex& target) override;
	virtual void execute(const Graph& g, const Graph::Vertex& source, const Graph::Vertex& target) override;
	virtual void cleanUp() override;
	virtual void addDecoratedVersionOf(const Graph::Vertex& v) override;
	virtual DijkstraVertex& getDecoratedVersionOf(const Graph::Vertex& v) override;
	virtual void decorateVerticesOf(const Graph& g) override;
	void gatherDecoratedVerticesWithUndeterminedEstimate();
	void relaxEdgesLeaving(const DijkstraVertex& v, const Graph& g);
	void relaxEdge(const DijkstraVertex& start, DijkstraVertex& end, Graph::OutgoingEdge::Weight w);

private:
	PriorityQueue undeterminedEstimateVertices;
	Hash decoratedVertices;
};

#endif //__DIJKSTRA_SHORTEST_PATH_HEADER_INCLUDED__