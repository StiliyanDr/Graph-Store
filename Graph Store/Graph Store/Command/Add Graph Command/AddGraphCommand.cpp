#include "AddGraphCommand.h"
#include "../../Graph Factory/GraphFactory.h"
#include "../Command Registrator/CommandRegistrator.h"

static CommandRegistrator<AddGraphCommand> registrator("ADD-GRAPH", "Adds a graph with specified identifier and type");

const String AddGraphCommand::DEFAULT_GRAPH_TYPE = "undirected";

void AddGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	addGraph(graphID, graphType);
}

void AddGraphCommand::parseArguments(args::Subparser& parser)
{
	args::Positional<String, StringReader> id(parser, "graph id", "The identifier of the new graph");
	args::Positional<String, StringReader> type(parser, "graph type", "The type of the new graph");

	parser.Parse();

	setGraphID(id);
	setGraphType(type);
}

void AddGraphCommand::setGraphID(args::Positional<String, StringReader>& id)
{
	if (id.Matched())
	{
		graphID = args::get(id);
	}
	else
	{
		throw std::runtime_error("Missing argument: [graph id]!");
	}
}

void AddGraphCommand::setGraphType(args::Positional<String, StringReader>& type)
{
	graphType = type.Matched() ? args::get(type) : DEFAULT_GRAPH_TYPE;
}

void AddGraphCommand::addGraph(const String& id, const String& type)
{
	std::unique_ptr<Graph> newGraph = tryToCreateGraph(id, type);
	GraphCollection& graphs = getGraphs();
	graphs.add(std::move(newGraph));
}

std::unique_ptr<Graph> AddGraphCommand::tryToCreateGraph(const String& id,
														 const String& type)
{
	try
	{
		return GraphFactory::instance().createGraph(type, id);
	}
	catch (std::bad_alloc&)
	{
		throw std::runtime_error("Not enough memory to add a graph!");
	}
}
