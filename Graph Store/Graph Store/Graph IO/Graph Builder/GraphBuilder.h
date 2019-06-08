#ifndef __GRAPH_BUILDER_HEADER_INCLUDED__
#define __GRAPH_BUILDER_HEADER_INCLUDED__

#include "File Parser/FileParser.h"
#include "String/String.h"
#include "Dynamic Array/DynamicArray.h"
#include "Graph/Abstract class/Graph.h"

namespace GraphIO
{
	class GraphBuilder
	{
		struct RawEdge
		{
			std::size_t startIDIndex;
			std::size_t endIDIndex;
			Graph::Edge::Weight weight;
		};

	public:
		GraphBuilder() = default;
		GraphBuilder(const GraphBuilder&) = delete;
		GraphBuilder& operator=(const GraphBuilder&) = delete;

		std::unique_ptr<Graph> buildFromFile(const String& fileName);

	private:
		void openFile(const String& name);
		void tryToBuildAGraphFromOpenedFile(const String& fileName);
		void buildAGraph();
		void createEmptyGraph();
		void addVerticesToTheCreatedGraph();
		void addEdgesHavingAddedVertices();
		RawEdge parseEdge();
		void addEdge(const RawEdge& e);
		unsigned parseUnsignedAndSkipUntil(char c);
		void releaseResources();
		auto createResourceReleaser();

	private:
		std::unique_ptr<Graph> graph;
		DynamicArray<String> identifiers;
		FileParser fileParser;
	};

	inline auto GraphBuilder::createResourceReleaser()
	{
		auto deleter =
			[](GraphBuilder* builder) { builder->releaseResources(); };
		auto releaser =
			std::unique_ptr<GraphBuilder, decltype(deleter)>(this, deleter);

		return releaser;
	}

}

#endif //__GRAPH_BUILDER_HEADER_INCLUDED__