#ifndef __USE_GRAPH_COMMAND_HEADER_INCLUDED__
#define __USE_GRAPH_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class UseGraphCommand : public Command
{
public:
	UseGraphCommand() = default;
	UseGraphCommand(const UseGraphCommand&) = delete;
	UseGraphCommand& operator=(const UseGraphCommand&) = delete;
	UseGraphCommand(UseGraphCommand&&) = delete;
	UseGraphCommand& operator=(UseGraphCommand&&) = delete;
	virtual ~UseGraphCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void useGraph(const String& id);

private:
	void parseArguments(args::Subparser& parser);
	void setGraphID(args::Positional<String, StringReader>& id);

private:
	String graphID;
};

#endif //__USE_GRAPH_COMMAND_HEADER_INCLUDED__