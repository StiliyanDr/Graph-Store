#ifndef __GRAPH_SAVER_HEADER_INCLUDED__
#define __GRAPH_SAVER_HEADER_INCLUDED__

#include <fstream>
#include <unordered_map>
#include "../Hash/Hash Function/HashFunctionStringSpecialization.h"
#include "../Graph/Abstract class/Graph.h"

class GraphSaver
{
	using VertexID = String;
	using Map = std::unordered_map<std::reference_wrapper<const VertexID>, size_t, HashFunction<VertexID>>;

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