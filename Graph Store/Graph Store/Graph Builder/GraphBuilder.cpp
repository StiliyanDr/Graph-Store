#include "GraphBuilder.h"
#include "../File Parser/Open File Fail Exception/OpenFileFailException.h"
#include "../Graph Builder/Graph Builder Exception/GraphBuilderException.h"
#include "../Graph Factory/GraphFactory.h"

std::unique_ptr<Graph> GraphBuilder::buildFromFile(const char* fileName)
{
	try
	{
		tryToParse(fileName);
		buildResultFromParsedFile();
	}
	catch (std::bad_alloc&)
	{
		dealWithBadAllocWhileWorkingWith(fileName);
	}

	clean();

	return std::move(graph);
}

void GraphBuilder::tryToParse(const char* fileName)
{
	try
	{
		parse(fileName);
	}
	catch (OpenFileFailException& ex)
	{
		throw GraphBuilderException(ex.what());
	}
	catch (FileParserException& ex)
	{
		clean();
		String message = "Error in file " + String(fileName) + "! ";
		throw GraphBuilderException(message + ex.what());
	}
}

void GraphBuilder::parse(const char* fileName)
{
	fileParser.openFile(fileName);
	parseTypeAndID();
	parseComponents();
	fileParser.closeFile();
}

void GraphBuilder::parseTypeAndID()
{
	graphType = fileParser.readLine();
	graphID = fileParser.readLine();
}

void GraphBuilder::parseComponents()
{
	identifiers = readIdentifiers();
	rawEdges = parseEdges();
}

DynamicArray<String> GraphBuilder::readIdentifiers()
{
	unsigned identifiersCount = parseUnsignedAndSkipUntil(NEW_LINE);
	DynamicArray<String> identifiers(identifiersCount, identifiersCount);

	for (size_t i = 0; i < identifiersCount; ++i)
	{
		identifiers[i] = fileParser.readLine();
	}

	return identifiers;
}

DynamicArray<GraphBuilder::RawEdge> GraphBuilder::parseEdges()
{
	unsigned edgesCount = parseUnsignedAndSkipUntil(NEW_LINE);
	DynamicArray<RawEdge> edges(edgesCount, edgesCount);

	for (size_t i = 0; i < edgesCount; ++i)
	{
		edges[i] = parseEdge();
	}

	return edges;
}

GraphBuilder::RawEdge GraphBuilder::parseEdge()
{
	RawEdge edge;

	fileParser.skipUntil(LEFT_PARENTHESIS);
	edge.startIDIndex = parseUnsignedAndSkipUntil(COMMA);
	edge.endIDIndex = parseUnsignedAndSkipUntil(COMMA);
	edge.weight = parseUnsignedAndSkipUntil(RIGHT_PARENTHESIS);
	fileParser.skipUntil(NEW_LINE);

	return edge;
}

unsigned GraphBuilder::parseUnsignedAndSkipUntil(char symbol)
{
	unsigned result = fileParser.parseUnsigned();
	fileParser.skipUntil(symbol);

	return result;
}

void GraphBuilder::buildResultFromParsedFile()
{
	createGraph();
	addVertices();
	addEdges();
}

void GraphBuilder::createGraph()
{
	graph = GraphFactory::createGraph(graphType, graphID);
}

void GraphBuilder::addVertices()
{
	size_t identifiersCount = identifiers.getCount();

	for (size_t i = 0; i < identifiersCount; ++i)
	{
		graph->addVertex(identifiers[i]);
	}
}

void GraphBuilder::addEdges()
{
	size_t edgesCount = rawEdges.getCount();

	for (size_t i = 0; i < edgesCount; ++i)
	{
		addEdge(rawEdges[i]);
	}
}

void GraphBuilder::addEdge(const RawEdge& rawEdge)
{
	String& startVertexID = identifiers[rawEdge.startIDIndex];
	String& endVertexID = identifiers[rawEdge.endIDIndex];
	Vertex& startVertex = graph->getVertexWithIdentifier(startVertexID);
	Vertex& endVertex = graph->getVertexWithIdentifier(endVertexID);
	graph->addEdgeBetweenWithWeight(startVertex, endVertex, rawEdge.weight);
}

void GraphBuilder::dealWithBadAllocWhileWorkingWith(const char* fileName)
{
	assert(fileName != nullptr);

	graph = nullptr;
	clean();

	throw GraphBuilderException("Not enough memory to load graph in " + String(fileName));
}

void GraphBuilder::clean()
{
	graphType = "";
	graphID = "";
	identifiers.empty();
	rawEdges.empty();
	fileParser.closeFile();
}
