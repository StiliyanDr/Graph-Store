#ifndef __GRAPH_SAVER_HEADER_INCLUDED__
#define __GRAPH_SAVER_HEADER_INCLUDED__

#include <fstream>
#include <unordered_map>
#include "../Hash/Hash Function/HashFunctionStringSpecialization.h"
#include "../Graph/Abstract class/Graph.h"

class GraphSaver
{
	using VertexID = String;
	
	struct IndexedVertex
	{	
		IndexedVertex(size_t index, const VertexID& id) :
			index(index), id(id) { }

		size_t index;
		const VertexID& id;
	};

	using Map = std::unordered_map<VertexID, IndexedVertex, HashFunction<VertexID>>;

public:
	void save(const Graph& graph);

private:
	void decorateVerticesOf(const Graph& g);
	void openFileFor(const Graph& g);
	String getFileNameFor(const Graph& g);
	void verifyFileIsOpen(const String& fileName);
	void saveDecoratedGraph(const Graph& g);
	void saveIDAndTypeOf(const Graph& g);
	void saveVerticesOf(const Graph& g);
	void saveEdgesOf(const Graph& g);
	size_t getIndexOf(const Graph::Vertex& v) const;
	void releaseResources();

private:
	std::ofstream file;
	Map decoratedVertices;
};

#endif //__GRAPH_SAVER_HEADER_INCLUDED__