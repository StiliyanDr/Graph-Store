#ifndef __LOAD_COMMAND_HEADER_INCLUDED__
#define __LOAD_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../../String/String.h"
#include "../String Reader/StringReader.h"

class LoadCommand : public Command
{
public:
	LoadCommand() = default;
	LoadCommand(const LoadCommand&) = delete;
	LoadCommand& operator=(const LoadCommand&) = delete;
	LoadCommand(LoadCommand&&) = delete;
	LoadCommand& operator=(LoadCommand&&) = delete;
	virtual ~LoadCommand() = default;

	virtual void execute(args::Subparser& parser) override;

private:
	static void loadGraphs(const String& path);

private:
	void parseArguments(args::Subparser& parser);
	void setPath(args::Positional<String, StringReader>& path);

private:
	String path;
};

#endif //__LOAD_COMMAND_HEADER_INCLUDED__