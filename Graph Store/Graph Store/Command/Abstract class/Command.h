#ifndef __COMMAND_HEADER_INCLUDED__
#define __COMMAND_HEADER_INCLUDED__

#include "../../../../../args-master/args-master/args.hxx"

class String;
class Graph;
class GraphCollection;

class Command
{
	friend class Application;

public:
	Command(const Command&) = delete;
	Command& operator=(const Command&) = delete;
	virtual ~Command() = default;
	virtual void execute(args::Subparser& parser) = 0;

protected:
	Command() = default;

protected:
	static GraphCollection& getGraphs();
	static bool isUsedGraph(const String& id);
	static Graph& getUsedGraph();
	static void useNoGraph();
	static void setUsedGraph(Graph& g);

private:
	static void setGraphs(GraphCollection& graphs);

private:
	static Graph* usedGraph;
	static GraphCollection* graphs;
};

#endif //__COMMAND_HEADER_INCLUDED__