#ifndef __ADD_VERTEX_COMMAND_HEADER_INCLUDED__
#define __ADD_VERTEX_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class AddVertexCommand : public Command
{
public:
	AddVertexCommand() = default;
	AddVertexCommand(const AddVertexCommand&) = delete;
	AddVertexCommand& operator=(const AddVertexCommand&) = delete;
	AddVertexCommand(AddVertexCommand&&) = delete;
	AddVertexCommand& operator=(AddVertexCommand&&) = delete;
	virtual ~AddVertexCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void addVertex(const String& id);

private:
	void parseArguments(args::Subparser& parser);
	void setVertexID(args::Positional<String, StringReader>& id);

private:
	String vertexID;
};

#endif //__ADD_VERTEX_COMMAND_HEADER_INCLUDED__