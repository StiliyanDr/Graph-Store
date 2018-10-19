#ifndef __DIJKSTRA_SHORTEST_PATH_HEADER_INCLUDED__
#define __DIJKSTRA_SHORTEST_PATH_HEADER_INCLUDED__

#include "../Abstract class/ShortestPathAlgorithm.h"
#include "../../Priority Queue/PriorityQueue.h"
#include "../../Hash/Hash Function/HashFunctionStringSpecialization.h"
#include <unordered_map>

class DijkstraShortestPath : public ShortestPathAlgorithm
{
	struct DijkstraVertex : public DecoratedVertex
	{
		DijkstraVertex() = default;
		DijkstraVertex(const Vertex& originalVertex) :
			DecoratedVertex(originalVertex)
		{
		}

		PriorityQueueHandle handle;
	};

	class KeyAccessor
	{
	public:
		const Distance& getKeyOf(const DijkstraVertex& v) const
		{
			return v.distanceToSource;
		}

		void setKeyOfWith(DijkstraVertex& v, const Distance& d) const
		{
			v.distanceToSource = d;
		}
	};

	class HandleUpdator
	{
	public:
		void operator()(DijkstraVertex& v, const PriorityQueueHandle& h) const
		{
			v.handle = h;
		}
	};

	typedef PriorityQueue<DijkstraVertex*, Less, Distance, KeyAccessor, HandleUpdator> PriorityQueue;
	typedef std::unordered_map<String, DijkstraVertex, HashFunction<String>> Hash;

	class HashIterator
	{
	public:
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

	private:
		Hash::iterator iterator;
	};

public:
	explicit DijkstraShortestPath(const String& id);
	DijkstraShortestPath(const DijkstraShortestPath&) = delete;
	DijkstraShortestPath& operator=(const DijkstraShortestPath&) = delete;

private:
	virtual void initialise(const Graph& graph, const Vertex& source, const Vertex& target) override;
	virtual void execute(const Graph& graph, const Vertex& source, const Vertex& target) override;
	virtual void cleanUp() override;
	virtual void addDecoratedVersionOf(const Vertex& vertex) override;
	virtual DijkstraVertex& getDecoratedVersionOf(const Vertex& vertex) override;
	void decorateVerticesOf(const Graph& graph);
	void gatherDecoratedVerticesWithUndeterminedEstimate();
	void relaxEdgesLeaving(const DijkstraVertex& vertex, const Graph& graph);
	void relaxEdge(const DijkstraVertex& start, DijkstraVertex& end, unsigned weight);

private:
	PriorityQueue undeterminedEstimateVertices;
	Hash decoratedVertices;
};

#endif //__DIJKSTRA_SHORTEST_PATH_HEADER_INCLUDED__