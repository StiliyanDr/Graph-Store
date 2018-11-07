#ifndef __ADD_EDGE_COMMAND_HEADER_INCLUDED__
#define __ADD_EDGE_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class AddEdgeCommand : public Command
{
public:
	AddEdgeCommand() = default;
	AddEdgeCommand(const AddEdgeCommand&) = delete;
	AddEdgeCommand& operator=(const AddEdgeCommand&) = delete;
	AddEdgeCommand(AddEdgeCommand&&) = delete;
	AddEdgeCommand& operator=(AddEdgeCommand&&) = delete;
	virtual ~AddEdgeCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void addEdge(const String& startVertexID,
						const String& endVertexID,
						OutgoingEdge::Weight weight);

private:
	void parseArguments(args::Subparser& parser);
	void setStartVertexID(args::Positional<String, StringReader>& id);
	void setEndVertexID(args::Positional<String, StringReader>& id);
	void setWeight(args::Positional<OutgoingEdge::Weight>& weight);

private:
	static const OutgoingEdge::Weight DEFAULT_EDGE_WEIGHT = 1;

private:
	String startVertexID;
	String endVertexID;
	OutgoingEdge::Weight weight;
};

#endif //__ADD_EDGE_COMMAND_HEADER_INCLUDED__