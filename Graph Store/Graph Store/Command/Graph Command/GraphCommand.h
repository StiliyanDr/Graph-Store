#ifndef __GRAPH_COMMAND_HEADER_INCLUDED__
#define __GRAPH_COMMAND_HEADER_INCLUDED__

#include "Command\Abstract Class\Command.h"

class String;
class Graph;
class GraphCollection;

class GraphCommand : public Command
{
	friend class Application;

protected:
	GraphCommand() = default;
	GraphCommand& operator=(const GraphCommand&) = delete;
	GraphCommand(const GraphCommand&) = delete;

protected:
	static GraphCollection& getGraphs();
	static Graph& getUsedGraph();
	static void setUsedGraph(Graph& g);
	static void useNoGraph();
	static bool isUsedGraph(const String& id);

private:
	static void setGraphs(GraphCollection& graphs);

private:
	static Graph* usedGraph;
	static GraphCollection* graphs;
};

#endif //__GRAPH_COMMAND_HEADER_INCLUDED__