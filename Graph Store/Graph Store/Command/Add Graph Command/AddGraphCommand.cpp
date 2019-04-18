#include "AddGraphCommand.h"
#include "Graph Factory/GraphFactory.h"
#include "Graph Collection/GraphCollection.h"
#include "Command/Command Registrator/CommandRegistrator.h"
#include "Command/Exceptions/CommandException.h"

static CommandRegistrator<AddGraphCommand> registrator("add-graph",
	                                                   "Adds a graph with specified id and type");

const String AddGraphCommand::DEFAULT_GRAPH_TYPE = "undirected";

void AddGraphCommand::parseArguments(args::Subparser& parser)
{
	PositionalString id(parser, "graph id", "The id of the new graph");
	PositionalString type(parser, "graph type", "The type of the new graph");

	parser.Parse();

	setGraphID(id);
	setGraphType(type);
}

void AddGraphCommand::setGraphID(PositionalString& id)
{
	graphID = getValueOf(id);
}

void AddGraphCommand::setGraphType(PositionalString& type)
{
	graphType = type.Matched() ? args::get(type) : DEFAULT_GRAPH_TYPE;
}

void AddGraphCommand::doExecute()
{
	GraphCollection::GraphPointer newGraph =
		tryToCreateGraph(graphID, graphType);
	GraphCollection& graphs = getGraphs();
	
	Graph* g = newGraph.get();
	graphs.add(std::move(newGraph));
	setUsedGraph(*g);
}

GraphCollection::GraphPointer
AddGraphCommand::tryToCreateGraph(const String& id, const String& type)
{
	try
	{
		return GraphFactory::instance().createGraph(type, id);
	}
	catch (std::bad_alloc&)
	{
		throw CommandException("Not enough memory!"_s);
	}
}
