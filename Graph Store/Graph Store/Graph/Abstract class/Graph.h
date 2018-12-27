#ifndef __GRAPH_HEADER_INCLUDED__
#define __GRAPH_HEADER_INCLUDED__

#include "../../String/String.h"
#include "../../Iterator/Iterator.h"
#include "../../Dynamic Array/DynamicArray.h"
#include "../../Hash/Hash.h"
#include "../../Hash/Hash Function/HashFunctionStringSpecialization.h"
#include "../../Hash/Identifier Accessor/IdentifierAccessor.h"
#include "../../Linked List/LinkedList.h"
#include <memory>

class Graph
{
public:
	class Vertex;

	class OutgoingEdge
	{
		friend class Graph;

	public:
		typedef unsigned Weight;

	public:
		const Vertex& getEnd() const;
		Vertex& getEnd();
		Weight getWeight() const;

	private:
		OutgoingEdge(Vertex& end, Weight w);

	private:
		Weight weight;
		Vertex& end;
	};

	class Edge
	{
		friend class Graph;

	public:
		typedef OutgoingEdge::Weight Weight;

	public:
		const Vertex& getStart() const;
		const Vertex& getEnd() const;
		Weight getWeight() const;

	private:
		Edge(const Vertex& start, const OutgoingEdge& e);

	private:
		const Vertex& start;
		const OutgoingEdge& incidentToStartEdge;
	};

private:
	using AdjacencyList = LinkedList<OutgoingEdge>;

public:
	class Vertex
	{
		friend class Graph;
		using AdjacencyListsIterator = LinkedList<AdjacencyList>::Iterator;

	public:
		Vertex(const Vertex&) = default;
		Vertex& operator=(const Vertex&) = default;
		~Vertex() = default;

		const String& getID() const;

	private:
		Vertex(String id, size_t index, const AdjacencyListsIterator& iterator);
		void setID(String id);
		void exchangeContentsWith(Vertex& v);

	private:
		String id;
		size_t index;
		AdjacencyListsIterator iterator;
	};

	using VerticesConstIterator = std::unique_ptr<ConstIterator<Vertex>>;
	using OutgoingEdgesConstIterator = std::unique_ptr<ConstIterator<OutgoingEdge>>;

private:
	using Hash = Hash<Vertex, String, IdentifierAccessor>;
	using Array = DynamicArray<Vertex>;

protected:
	using VerticesConcreteIterator = Array::Iterator;
	using VerticesConcreteConstIterator = Array::ConstIterator;
	using OutgoingEdgesConcreteIterator = AdjacencyList::Iterator;
	using OutgoingEdgesConcreteConstIterator = AdjacencyList::ConstIterator;

protected:
	class EdgesConstIteratorBase
	{
	public:
		EdgesConstIteratorBase(const VerticesConcreteConstIterator& verticesIterator,
			                   const OutgoingEdgesConcreteConstIterator& edgesIterator);
		EdgesConstIteratorBase(const EdgesConstIteratorBase&) = default;
		EdgesConstIteratorBase& operator=(const EdgesConstIteratorBase&) = default;
		virtual ~EdgesConstIteratorBase() = default;

		EdgesConstIteratorBase& operator++();
		Edge operator*() const;
		operator bool() const;
		bool operator!() const;

	protected:
		bool pointsToEdge() const;

	private:
		virtual bool pointsToUniteratedEdge() const;
		void skipIteratedEdges();
		void goToNextEdge();
		void goToNextListIfCurrentOneEnded();
		bool thereAreMoreEdges() const;

	private:
		VerticesConcreteConstIterator verticesIterator;
		OutgoingEdgesConcreteConstIterator edgesIterator;
	};

public:
	using EdgesConstIterator = std::unique_ptr<EdgesConstIteratorBase>;

public:
	virtual ~Graph() = default;

	void addVertex(const String& id);
	void removeVertex(Vertex& v);
	virtual void addEdge(Vertex& start, Vertex& end, Edge::Weight weight) = 0;
	virtual void removeEdge(const Vertex& start, const Vertex& end) = 0;

	Vertex& getVertexWithID(const String& id);
	VerticesConstIterator getConstIteratorOfVertices() const;
	OutgoingEdgesConstIterator getConstIteratorOfEdgesLeaving(const Vertex& v) const;
	virtual EdgesConstIterator getConstIteratorOfEdges() const = 0;
	virtual unsigned getEdgesCount() const;
	unsigned getVerticesCount() const;

	bool hasVertexWithID(const String& id) const;
	bool hasEdge(const Vertex& start, const Vertex& end) const;

	const String& getID() const;

protected:
	Graph(const String& id);
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;

	void removeEdgeFromTo(const Vertex& start, const Vertex& end);
	void addEdgeFromTo(const Vertex& start, Vertex& end, OutgoingEdge::Weight weight);
	void verifyOwnershipOf(const Vertex& v) const;
	bool isOwnerOf(const Vertex& v) const;
	VerticesConcreteIterator getConcreteIteratorOfVertices();
	VerticesConcreteConstIterator getConcreteConstIteratorOfVertices() const;
	OutgoingEdgesConcreteIterator getConcreteIteratorOfEdgesLeaving(const Vertex& v);
	OutgoingEdgesConcreteConstIterator getConcreteConstIteratorOfEdgesLeaving(const Vertex& v) const;
	template <class ConcreteIterator>
	EdgesConstIterator createConstIteratorOfEdges() const;

private:
	virtual void removeEdgesEndingIn(const Vertex& v) = 0;
	virtual void removeEdgesLeaving(const Vertex& v);
	void tryToAddNewVertex(const String& id);
	void addVertexToCollection(const Vertex& v);
	void removeVertexFromCollection(Vertex& v);
	OutgoingEdgesConcreteIterator searchForEdgeFromTo(const Vertex& start, const Vertex& end);
	AdjacencyList& getEdgesLeaving(const Vertex& v);
	const AdjacencyList& getEdgesLeaving(const Vertex& v) const;
	Vertex createVertex(String id);
	void setID(String id);

private:
	static const size_t INITIAL_COLLECTION_SIZE = 16;

private:
	String id;
	Array vertices;
	LinkedList<AdjacencyList> adjacencyLists;
	Hash vertexSearchSet;
};

bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs);

#endif //__GRAPH_HEADER_INCLUDED__