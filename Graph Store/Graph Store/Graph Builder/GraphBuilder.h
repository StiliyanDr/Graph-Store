#ifndef __GRAPH_BUILDER_HEADER_INCLUDED__
#define __GRAPH_BUILDER_HEADER_INCLUDED__

#include "../File Parser/FileParser.h"
#include "../String/String.h"
#include "../Dynamic Array/DynamicArray.h"
#include "../Graph/Abstract class/Graph.h"

class GraphBuilder
{
	struct RawEdge
	{
		size_t startIDIndex;
		size_t endIDIndex;
		unsigned weight;
	};

public:
	GraphBuilder() = default;
	GraphBuilder(const GraphBuilder&) = delete;
	GraphBuilder& operator=(const GraphBuilder&) = delete;
	GraphBuilder(GraphBuilder&&) = delete;
	GraphBuilder& operator=(GraphBuilder&&) = delete;
	~GraphBuilder() = default;

	std::unique_ptr<Graph> buildFromFile(const String& fileName);

private:
	void tryToParse(const String& fileName);
	void parse(const String& fileName);
	void parseTypeAndID();
	void parseComponents();
	DynamicArray<String> readIdentifiers();
	DynamicArray<RawEdge> parseEdges();
	RawEdge parseEdge();
	unsigned parseUnsignedAndSkipUntil(char symbol);
	void buildResultFromParsedFile();
	void createGraph();
	void addVertices();
	void addEdges();
	void addEdge(const RawEdge& rawEdge);
	void clean();
	void dealWithBadAllocWhileWorkingWith(const String& fileName);

private:
	static const char EDGE_START = '(';
	static const char EDGE_END = ')';
	static const char EDGE_ATTRIBUTE_SEPARATOR = ',';
	static const char NEW_LINE = '\n';

private:
	std::unique_ptr<Graph> graph;
	String graphType;
	String graphID;
	DynamicArray<String> identifiers;
	DynamicArray<RawEdge> rawEdges;
	FileParser fileParser;
};

#endif //__GRAPH_BUILDER_HEADER_INCLUDED__