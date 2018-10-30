#ifndef __GRAPH_BASE_HEADER_INCLUDED__
#define __GRAPH_BASE_HEADER_INCLUDED__

#include "../../Dynamic Array/DynamicArray.h"
#include "../../Hash/Hash.h"
#include "../../Hash/Hash Function/HashFunctionStringSpecialization.h"
#include "../../Hash/Identifier Accessor/IdentifierAccessor.h"
#include "../Abstract class/Graph.h"

class GraphBase : public Graph
{
	typedef Hash<Vertex, String, IdentifierAccessor> Hash;
	typedef DynamicArray<Vertex*> Array;

protected:
	typedef DynamicArray<Vertex*>::Iterator VerticesConcreteIterator;
	typedef LinkedList<OutgoingEdge>::Iterator OutgoingEdgesConcreteIterator;

public:
	GraphBase(const GraphBase&) = delete;
	GraphBase& operator=(const GraphBase&) = delete;
	virtual ~GraphBase();

	virtual void addVertex(const String& id) override;
	virtual void removeVertex(Vertex& v) override;

	virtual Vertex& getVertexWithID(const String& id) override;
	virtual VerticesConstIterator getConstIteratorOfVertices() const override;
	virtual OutgoingEdgesConstIterator getConstIteratorOfEdgesLeaving(const Vertex& v) const override;
	virtual unsigned getVerticesCount() const override;

protected:
	GraphBase(const String& id);

	virtual void removeEdgesEndingIn(Vertex& v) = 0;
	virtual void removeEdgesLeaving(Vertex& v);
	void removeEdgeFromTo(Vertex& start, const Vertex& end);
	void addEdgeFromTo(Vertex& start, Vertex& end, unsigned weight);
	bool hasEdgeFromTo(const Vertex& start, const Vertex& end) const;
	bool hasVertexWithID(const String& id) const;
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
	Array vertices;
	Hash vertexSearchSet;
};

#endif //__GRAPH_BASE_HEADER_INCLUDED__