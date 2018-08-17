#include "GraphBuilder.h"
#include "../File Parser/Open File Fail Exception/OpenFileFailException.h"
#include "../Graph Builder/Graph Builder Exception/GraphBuilderException.h"
#include "../Graph Factory/GraphFactory.h"

std::unique_ptr<Graph> GraphBuilder::buildFromFile(const String& fileName)
{
	openFile(fileName);
	
	try
	{
		buildAGraph();
	}
	catch (std::exception& e)
	{
		handleExceptionDuringBuilding(fileName, e);
	}

	clean();

	return std::move(graph);
}

void GraphBuilder::openFile(const String& name)
{
	assert(!fileParser.hasOpenedFile());

	try
	{
		fileParser.openFile(name);
	}
	catch (OpenFileFailException& e)
	{
		throw GraphBuilderException(e.what());
	}
}

void GraphBuilder::buildAGraph()
{
	createEmptyGraph();
	addVerticesToTheCreatedGraph();
	addEdgesHavingAddedVertices();
}

void GraphBuilder::createEmptyGraph()
{
	assert(graph == nullptr);

	String id = fileParser.readLine();
	String type = fileParser.readLine();

	graph = GraphFactory::instance().createGraph(type, id);
}

void GraphBuilder::addVerticesToTheCreatedGraph()
{
	assert(graph != nullptr);
	assert(identifiers.isEmpty());

	unsigned identifiersCount = parseUnsignedAndSkipUntil(NEW_LINE);
	identifiers.ensureSize(identifiersCount);

	for (unsigned i = 0; i < identifiersCount; ++i)
	{
		identifiers.add(fileParser.readLine());

		graph->addVertex(identifiers[i]);
	}
}

void GraphBuilder::addEdgesHavingAddedVertices()
{
	assert(graph != nullptr);

	unsigned edgesCount = parseUnsignedAndSkipUntil(NEW_LINE);
	RawEdge rawEdge;

	for (unsigned i = 1; i <= edgesCount; ++i)
	{
		rawEdge = parseEdge();
		addEdge(rawEdge);
	}
}

GraphBuilder::RawEdge GraphBuilder::parseEdge()
{
	RawEdge edge;

	fileParser.skipUntil(EDGE_START);
	edge.startIDIndex = parseUnsignedAndSkipUntil(EDGE_ATTRIBUTE_SEPARATOR);
	edge.endIDIndex = parseUnsignedAndSkipUntil(EDGE_ATTRIBUTE_SEPARATOR);
	edge.weight = parseUnsignedAndSkipUntil(EDGE_END);
	fileParser.skipUntil(NEW_LINE);

	return edge;
}

unsigned GraphBuilder::parseUnsignedAndSkipUntil(char symbol)
{
	unsigned result = fileParser.parseUnsigned();
	fileParser.skipUntil(symbol);

	return result;
}

void GraphBuilder::addEdge(const RawEdge& edge)
{
	String& startID = identifiers[edge.startIDIndex];
	String& endID = identifiers[edge.endIDIndex];

	Vertex& start = graph->getVertexWithID(startID);
	Vertex& end = graph->getVertexWithID(endID);

	graph->addEdgeBetweenWithWeight(start, end, edge.weight);
}

void GraphBuilder::handleExceptionDuringBuilding(const String& fileName, const std::exception& e)
{
	graph = nullptr;
	clean();

	throw GraphBuilderException(e.what() + String("\nError in: ") + fileName);
}

void GraphBuilder::clean()
{
	identifiers.empty();
	fileParser.closeFile();
}
