#ifndef __GRAPH_COLLECTION_HEADER_INCLUDED__
#define __GRAPH_COLLECTION_HEADER_INCLUDED__

#include "Graph/Abstract class/Graph.h"
#include "Iterator/STLIteratorAdapter.h"
#include <vector>
#include <memory>
#include <algorithm>

class GraphCollection
{
public:
	using GraphPointer = std::unique_ptr<Graph>;

private:
	using Collection = std::vector<GraphPointer>;

public:
	template <bool isConst = false>
	class GraphCollectionIterator : public AbstractIterator<Graph, isConst>
	{
		friend class GraphCollection;

		using STLIteratorType =
			std::conditional_t<isConst, Collection::const_iterator, Collection::iterator>;
		using IteratorType = STLIteratorAdapter<STLIteratorType>;

	public:
		using typename AbstractIterator<Graph, isConst>::Reference;

	public:
		GraphCollectionIterator<isConst>& operator++() override
		{
			++iterator;

			return *this;
		}

	private:
		GraphCollectionIterator(STLIteratorType iterator, STLIteratorType end) :
			iterator(iterator, end)
		{
		}

		Reference getCurrentItem() const override
		{
			assert(isValid());

			return *(*iterator);
		}

		bool isValid() const override
		{
			return static_cast<bool>(iterator);
		}

	private:
		IteratorType iterator;
	};

	using Iterator = GraphCollectionIterator<false>;
	using ConstIterator = GraphCollectionIterator<true>;

public:
	GraphCollection() = default;
	explicit GraphCollection(std::size_t size);
	GraphCollection(const GraphCollection&) = delete;
	GraphCollection& operator=(const GraphCollection&) = delete;
	GraphCollection(GraphCollection&&) = default;
	GraphCollection& operator=(GraphCollection&&) = default;

	void add(GraphPointer graph);
	GraphPointer remove(const String& id);

	Graph& operator[](const String& id);
	const Graph& operator[](const String& id) const;
	bool contains(const String& id) const;

	Iterator getIterator() noexcept;
	ConstIterator getConstIterator() const noexcept;

	std::size_t getCount() const noexcept;
	bool isEmpty() const noexcept;
	void empty();

private:
	static void verifyPointerIsNotNull(const GraphPointer& p);
	template <class ForwardIterator>
	static ForwardIterator getGraph(ForwardIterator begin,
		                            ForwardIterator end,
		                            const String& id);
	template <class ForwardIterator>
	static ForwardIterator findGraph(ForwardIterator begin,
		                             ForwardIterator end,
		                             const String& id);
private:
	void tryToAdd(GraphPointer graph);

private:
	Collection graphs;
};

template <class ForwardIterator>
ForwardIterator GraphCollection::getGraph(ForwardIterator begin,
	                                      ForwardIterator end,
	                                      const String& id)
{
	ForwardIterator iterator = findGraph(begin, end, id);

	if (iterator != end)
	{
		return iterator;
	}
	else
	{
		std::string message = "There is no graph with id: ";
		throw std::out_of_range(message + id.cString());
	}
}

template <class ForwardIterator>
ForwardIterator GraphCollection::findGraph(ForwardIterator begin,
	                                       ForwardIterator end,
	                                       const String& id)
{
	return std::find_if(begin, end, [&id](const auto& graph)
	{
		assert(graph != nullptr);

		return graph->getID() == id;
	});
}

#endif //__GRAPH_COLLECTION_HEADER_INCLUDED__