#ifndef __GRAPH_BUILDER_HEADER_INCLUDED__
#define __GRAPH_BUILDER_HEADER_INCLUDED__

#include "../../File Parser/FileParser.h"
#include "../../String/String.h"
#include "../../Dynamic Array/DynamicArray.h"
#include "../../Graph/Abstract class/Graph.h"

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
		unsigned parseUnsignedAndSkipUntil(char symbol);
		void handleExceptionDuringBuilding(const String& fileName, const std::exception& e);
		void releaseResources();

	private:
		std::unique_ptr<Graph> graph;
		DynamicArray<String> identifiers;
		FileParser fileParser;
	};
}

#endif //__GRAPH_BUILDER_HEADER_INCLUDED__