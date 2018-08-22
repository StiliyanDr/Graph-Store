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
	typedef DynamicArray<Vertex*>::DynamicArrayIterator<Vertex*> VertexConcreteIterator;
	typedef LinkedList<Edge>::LinkedListIterator<Edge> EdgeConcreteIterator;

public:
	GraphBase(const GraphBase&) = delete;
	GraphBase& operator=(const GraphBase&) = delete;
	GraphBase(GraphBase&&) = delete;
	GraphBase& operator=(GraphBase&&) = delete;
	virtual ~GraphBase();

	virtual void addVertex(const String& id) override;
	virtual void removeVertex(Vertex& vertex) override;

	virtual Vertex& getVertexWithID(const String& id) override;
	virtual VertexAbstractIterator getIteratorOfVertices() override;
	virtual EdgeAbstractIterator getIteratorOfEdgesStartingFrom(Vertex& vertex) override;
	virtual unsigned getVerticesCount() const override;

protected:
	GraphBase(String id);

	virtual void removeEdgesEndingIn(Vertex& vertex);
	virtual void removeEdgesStartingFrom(Vertex& vertex);
	void removeEdgeFromTo(Vertex& start, const Vertex& end);
	void addEdgeFromToWithWeight(Vertex& start, Vertex& end, unsigned weight);
	bool hasEdgeFromTo(Vertex& start, const Vertex& end);
	bool hasVertexWithID(const String& id);
	Vertex* searchForVertexWithID(const String& id);
	bool isOwnerOf(const Vertex& vertex) const;

private:
	void tryToAddNewVertex(const String& id);
	void addVertexToCollection(std::unique_ptr<Vertex> vertex);
	void removeVertexFromCollection(const Vertex& vertex);
	EdgeConcreteIterator searchForEdgeFromTo(Vertex& start, const Vertex& end);
	EdgeConcreteIterator getConcreteIteratorOfEdgesStartingFrom(Vertex& vertex);
	LinkedList<Edge>& getEdgesStartingFrom(Vertex& vertex);
	VertexConcreteIterator getConcreteIteratorOfVertices();
	std::unique_ptr<Vertex> createVertex(const String& id) const;
	void destroyAllVertices();
	void destroyVertex(Vertex* vertex) const;

private:
	static const size_t INITIAL_COLLECTION_SIZE = 16;

private:
	Array vertices;
	Hash vertexSearchSet;
};

#endif //__GRAPH_BASE_HEADER_INCLUDED__