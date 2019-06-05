#ifndef __GRAPH_SAVER_HEADER_INCLUDED__
#define __GRAPH_SAVER_HEADER_INCLUDED__

#include <fstream>
#include <unordered_map>
#include "Hash/Hash Function/HashFunction.h"
#include "Graph/Abstract class/Graph.h"

namespace GraphIO
{
	class GraphSaver
	{
		using VertexID = String;
		using Map = std::unordered_map<std::reference_wrapper<const VertexID>, std::size_t, HashFunction<VertexID>>;

	public:
		void save(const Graph& graph);

	private:
		void decorateVerticesOf(const Graph& g);
		void openFileFor(const Graph& g);
		void verifyFileIsOpen(const String& fileName);
		void saveDecoratedGraph(const Graph& g);
		void saveIDAndTypeOf(const Graph& g);
		void saveVerticesOf(const Graph& g);
		void saveEdgesOf(const Graph& g);
		std::size_t getIndexOf(const Graph::Vertex& v) const;
		void releaseResources();

	private:
		std::ofstream file;
		Map decoratedVertices;
	};
}

#endif //__GRAPH_SAVER_HEADER_INCLUDED__