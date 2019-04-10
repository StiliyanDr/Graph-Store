#ifndef __REMOVE_VERTEX_COMMAND_HEADER_INCLUDED__
#define __REMOVE_VERTEX_COMMAND_HEADER_INCLUDED__

#include "Command\Graph Command\GraphCommand.h"
#include "String\String.h"

class RemoveVertexCommand : public GraphCommand
{
public:
	RemoveVertexCommand() = default;

private:
	void parseArguments(args::Subparser& parser) override;
	void doExecute() override;

private:
	String vertexID;
};

#endif //__REMOVE_VERTEX_COMMAND_HEADER_INCLUDED__