#ifndef __REMOVE_EDGE_COMMAND_HEADER_INCLUDED__
#define __REMOVE_EDGE_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class RemoveEdgeCommand : public Command
{
public:
	RemoveEdgeCommand() = default;
	RemoveEdgeCommand(const RemoveEdgeCommand&) = delete;
	RemoveEdgeCommand& operator=(const RemoveEdgeCommand&) = delete;
	RemoveEdgeCommand(RemoveEdgeCommand&&) = delete;
	RemoveEdgeCommand& operator=(RemoveEdgeCommand&&) = delete;
	virtual ~RemoveEdgeCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void removeEdge(const String& startVertexID, const String& endVertexID);

private:
	void parseArguments(args::Subparser& parser);
	void setStartVertexID(args::Positional<String, StringReader>& id);
	void setEndVertexID(args::Positional<String, StringReader>& id);

private:
	String startVertexID;
	String endVertexID;
};

#endif //__REMOVE_EDGE_COMMAND_HEADER_INCLUDED__