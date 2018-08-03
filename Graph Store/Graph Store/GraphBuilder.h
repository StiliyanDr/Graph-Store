#ifndef __GRAPH_BUILDER_HEADER_INCLUDED__
#define __GRAPH_BUILDER_HEADER_INCLUDED__

#include "FileParser.h"
#include "String.h"
#include "DynamicArray.h"
#include "Graph.h"

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

	std::unique_ptr<Graph> buildFromFile(const char* fileName);

private:
	void tryToParse(const char* fileName);
	void parse(const char* fileName);
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

private:
	static const char LEFT_PARENTHESIS = '(';
	static const char RIGHT_PARENTHESIS = ')';
	static const char COMMA = ',';
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