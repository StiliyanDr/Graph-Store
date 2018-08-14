#ifndef __REMOVE_VERTEX_COMMAND_HEADER_INCLUDED__
#define __REMOVE_VERTEX_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class RemoveVertexCommand : public Command
{
public:
	RemoveVertexCommand() = default;
	RemoveVertexCommand(const RemoveVertexCommand&) = delete;
	RemoveVertexCommand& operator=(const RemoveVertexCommand&) = delete;
	RemoveVertexCommand(RemoveVertexCommand&&) = delete;
	RemoveVertexCommand& operator=(RemoveVertexCommand&&) = delete;
	virtual ~RemoveVertexCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void removeVertex(const String& id);

private:
	void parseArguments(args::Subparser& parser);
	void setVertexID(args::Positional<String, StringReader>& id);

private:
	String vertexID;
};

#endif //__REMOVE_VERTEX_COMMAND_HEADER_INCLUDED__