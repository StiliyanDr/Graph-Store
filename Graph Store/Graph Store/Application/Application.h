#ifndef __APPLICATION_HEADER_INCLUDED__
#define __APPLICATION_HEADER_INCLUDED__

#include "../../../../args-master/args-master/args.hxx"
#include "Graph Collection/GraphCollection.h"
#include "String Cutter/StringCutter.h"
#include <forward_list>

class Command;

class Application
{
	using Function = std::function<void(args::Subparser&)>;
	using ArgumentsList = std::vector<std::string>;
	using Delimiters = std::initializer_list<char>;

public:
	static Application& instance();

public:
	void runIn(const String& directory);
	void addCommand(const char* name,
		            const char* description,
		            Command& command);

private:
	static void verifyStringIsNotNull(const char* s);
	static void setCurrentDirectory(const String& path);

private:
	Application();
	Application(const Application&);
	Application& operator=(const Application&);
	~Application() = default;

	void loadGraphsFrom(const String& directory);
	void passLoadedGraphsToGraphCommands();
	void interact();
	void invokeCommand(char* commandLine);
	void addExitCommand();
	void addHelpCommand();
	void addCommand(const char* name,
		            const char* description,
		            Function function);

private:
	static const String COMMAND_PROMPT;

private:
	args::ArgumentParser parser;
	args::Group commandsGroup;
	std::forward_list<args::Command> commands;
	StringCutter cutter;
	bool receivedExitCommand;
	GraphCollection graphs;
};

#endif //__APPLICATION_HEADER_INCLUDED__