#ifndef __ADD_VERTEX_COMMAND_HEADER_INCLUDED__
#define __ADD_VERTEX_COMMAND_HEADER_INCLUDED__

#include "Command/Graph Command/GraphCommand.h"
#include "String/String.h"

class AddVertexCommand : public GraphCommand
{
public:
	AddVertexCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

private:
	String vertexID;
};

#endif //__ADD_VERTEX_COMMAND_HEADER_INCLUDED__