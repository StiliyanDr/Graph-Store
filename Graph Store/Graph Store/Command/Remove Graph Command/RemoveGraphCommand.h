#ifndef __REMOVE_GRAPH_COMMAND_HEADER_INCLUDED__
#define __REMOVE_GRAPH_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class RemoveGraphCommand : public Command
{
public:
	RemoveGraphCommand() = default;
	RemoveGraphCommand(const RemoveGraphCommand&) = delete;
	RemoveGraphCommand& operator=(const RemoveGraphCommand&) = delete;
	RemoveGraphCommand(RemoveGraphCommand&&) = delete;
	RemoveGraphCommand& operator=(RemoveGraphCommand&&) = delete;
	virtual ~RemoveGraphCommand() = default;
	
	virtual void execute(args::Subparser& parser) override;

private:
	static void removeGraph(const String& id);

private:
	void parseArguments(args::Subparser& parser);
	void setGraphID(args::Positional<String, StringReader>& id);

private:
	String graphID;
};

#endif //__REMOVE_GRAPH_COMMAND_HEADER_INCLUDED__