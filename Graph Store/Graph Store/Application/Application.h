#ifndef __APPLICATION_HEADER_INCLUDED__
#define __APPLICATION_HEADER_INCLUDED__

#include "../../../../args-master/args-master/args.hxx"
#include "../Linked List/LinkedList.h"

class Command;

class Application
{
	typedef std::function<void(args::Subparser&)> Function;

public:
	static Application& instance();

public:
	void run();
	void addCommand(const char* name, const char* description, Command& command);

private:
	Application();
	Application(const Application&);
	Application& operator=(const Application&);
	Application(Application&&);
	Application& operator=(Application&&);
	~Application() = default;

	void invokeCommand(char* commandLine);
	void addExitCommand();
	void addHelpCommand();
	void addCommand(const char* name, const char* description, Function function);

private:
	args::ArgumentParser parser;
	args::Group commandsGroup;
	LinkedList<args::Command> commands;
	bool receivedExitCommand;
};

#endif //__APPLICATION_HEADER_INCLUDED__