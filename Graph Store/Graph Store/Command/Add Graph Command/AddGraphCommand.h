#ifndef __ADD_GRAPH_COMMAND_HEADER_INCLUDED__
#define __ADD_GRAPH_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"
#include "Graph/Abstract class/Graph.h"

class AddGraphCommand : public Command
{
public:
	AddGraphCommand() = default;
	AddGraphCommand(const AddGraphCommand&) = delete;
	AddGraphCommand& operator=(const AddGraphCommand&) = delete;
	AddGraphCommand(AddGraphCommand&&) = delete;
	AddGraphCommand& operator=(AddGraphCommand&&) = delete;
	virtual ~AddGraphCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void addGraph(const String& id, const String& type);
	static std::unique_ptr<Graph> tryToCreateGraph(const String& id, const String& type);

private:
	void parseArguments(args::Subparser& parser);
	void setGraphID(args::Positional<String, StringReader>& id);
	void setGraphType(args::Positional<String, StringReader>& type);

private:
	static const String DEFAULT_GRAPH_TYPE;

private:
	String graphID;
	String graphType;
};

#endif //__ADD_GRAPH_COMMAND_HEADER_INCLUDED__