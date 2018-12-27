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
	virtual ~Graph();

	void addVertex(const String& id);
	void removeVertex(Vertex& v);
	virtual void addEdge(Vertex& start, Vertex& end, Edge::Weight weight) = 0;
	virtual void removeEdge(Vertex& start, Vertex& end) = 0;

	Vertex& getVertexWithID(const String& id);
	VerticesConstIterator getConstIteratorOfVertices() const;
	OutgoingEdgesConstIterator getConstIteratorOfEdgesLeaving(const Vertex& v) const;
	virtual EdgesConstIterator getConstIteratorOfEdges() const = 0;
	unsigned getVerticesCount() const;

	bool hasVertexWithID(const String& id) const;
	bool hasEdge(const Vertex& start, const Vertex& end) const;

	const String& getID() const;
	void setID(String id);

protected:
	Graph(const String& id);
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;

	virtual void removeEdgesEndingIn(Vertex& v) = 0;
	virtual void removeEdgesLeaving(Vertex& v);
	void removeEdgeFromTo(Vertex& start, const Vertex& end);
	void addEdgeFromTo(Vertex& start, Vertex& end, OutgoingEdge::Weight weight);
	void verifyOwnershipOf(const Vertex& v) const;
	bool isOwnerOf(const Vertex& v) const;
	VerticesConcreteIterator getConcreteIteratorOfVertices();
	OutgoingEdgesConcreteIterator getConcreteIteratorOfEdgesLeaving(Vertex& v);
	VerticesConcreteConstIterator getConcreteConstIteratorOfVertices() const;
	OutgoingEdgesConcreteConstIterator getConcreteConstIteratorOfEdgesLeaving(const Vertex& v) const;
	template <class ConcreteIterator>
	EdgesConstIterator createConstIteratorOfEdges() const;

private:
	void tryToAddNewVertex(const String& id);
	void addVertexToCollection(std::unique_ptr<Vertex> vertex);
	void removeVertexFromCollection(const Vertex& vertex);
	OutgoingEdgesConcreteIterator searchForEdgeFromTo(Vertex& start, const Vertex& end);
	LinkedList<OutgoingEdge>& getEdgesLeaving(Vertex& v);
	std::unique_ptr<Vertex> createVertex(const String& id) const;
	void destroyAllVertices();

private:
	static const size_t INITIAL_COLLECTION_SIZE = 16;

private:
	String id;
	Array vertices;
	Hash vertexSearchSet;
};

bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs);

#endif //__GRAPH_HEADER_INCLUDED__