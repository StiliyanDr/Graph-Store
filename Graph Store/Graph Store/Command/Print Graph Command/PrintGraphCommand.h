#ifndef __PRINT_GRAPH_COMMAND_HEADER_INCLUDED__
#define __PRINT_GRAPH_COMMAND_HEADER_INCLUDED__

#include "Command/Graph Command/GraphCommand.h"
#include "Graph/Abstract class/Graph.h"

class PrintGraphCommand : public GraphCommand
{
public:
	PrintGraphCommand() = default;

private:
	static void printEdgesLeaving(const Graph::Vertex& v, const Graph& g);
	static void printEdge(const Graph::OutgoingEdge& e);

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;
	void setGraphID(PositionalString& id);

private:
	String graphID;
};

#endif //__PRINT_GRAPH_COMMAND_HEADER_INCLUDED__