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
		const Vertex& getVertex() const;
		Vertex& getVertex();
		Weight getWeight() const;

	private:
		OutgoingEdge(Vertex& v, Weight w = 1);
		void setWeight(Weight w);

	private:
		Weight weight;
		Vertex& vertex;
	};

	class Vertex
	{
		friend class Graph;

	public:
		Vertex(const Vertex&) = delete;
		Vertex& operator=(const Vertex&) = delete;

		const String& getID() const;

	private:
		Vertex(String id, size_t index);
		void setID(String id);

	private:
		String id;
		size_t index;
		LinkedList<OutgoingEdge> edges;
	};

	class Edge
	{
	public:
		typedef OutgoingEdge::Weight Weight;
		
	public:
		Edge(Vertex& start, OutgoingEdge& e);

		const Vertex& getStart() const;
		const Vertex& getEnd() const;
		Weight getWeight() const;

	private:
		Vertex& start;
		OutgoingEdge& incidentToStartEdge;
	};

	typedef std::unique_ptr<ConstIterator<const Vertex*>> VerticesConstIterator;
	typedef std::unique_ptr<ConstIterator<OutgoingEdge>> OutgoingEdgesConstIterator;

protected:
	typedef DynamicArray<Vertex*>::Iterator VerticesConcreteIterator;
	typedef LinkedList<OutgoingEdge>::Iterator OutgoingEdgesConcreteIterator;

private:
	typedef Hash<Vertex, String, IdentifierAccessor> Hash;
	typedef DynamicArray<Vertex*> Array;

public:
	class EdgesConstIterator
	{
		friend class Graph;

	public:
		EdgesConstIterator& operator++();
		EdgesConstIterator operator++(int);
		Edge operator*() const;
		operator bool() const;
		bool operator!() const;

	private:
		EdgesConstIterator(const VerticesConcreteIterator& verticesIterator,
			               const OutgoingEdgesConcreteIterator& edgesIterator);

		void skipIteratedEdges();
		void goToNextEdge();
		void goToNextListIfCurrentOneEnded();
		bool pointsToUniteratedEdge() const;
		bool pointsToEdge() const;
		bool thereAreMoreEdges() const;

	private:
		VerticesConcreteIterator verticesIterator;
		OutgoingEdgesConcreteIterator edgesIterator;
	};

public:
	virtual ~Graph();

	void addVertex(const String& id);
	void removeVertex(Vertex& v);
	virtual void addEdge(Vertex& start, Vertex& end, Edge::Weight weight) = 0;
	virtual void removeEdge(Vertex& start, Vertex& end) = 0;

	Vertex& getVertexWithID(const String& id);
	VerticesConstIterator getConstIteratorOfVertices() const;
	OutgoingEdgesConstIterator getConstIteratorOfEdgesLeaving(const Vertex& v) const;
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