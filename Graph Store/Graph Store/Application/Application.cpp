#include "Application.h"
#include <assert.h>
#include <iostream>
#include "../String Cutter/StringCutter.h"
#include "../Logger/Logger.h"
#include "../Command/Abstract class/Command.h"

Application& Application::instance()
{
	static Application theInstance;
	
	return theInstance;
}

Application::Application() :
	parser("GRAPH STORE APPLICATION"),
	commandsGroup(parser, "SUPPORTED COMMANDS"),
	receivedExitCommand(false)
{
	addExitCommand();
	addHelpCommand();
}

void Application::addExitCommand()
{
	addCommand("EXIT", "Terminates the program", [&](args::Subparser& parser)
	{
		parser.Parse();
		receivedExitCommand = true;
	});
}

void Application::addHelpCommand()
{
	addCommand("HELP", "Lists the supported commands", [&](args::Subparser& parser)
	{
		parser.Parse();
		
		std::cout << "Supported commands:\n";

		auto iterator = commands.cbegin();
		auto invalidIterator = commands.cend();

		while (iterator != invalidIterator)
		{
			const args::Command& command = *iterator;

			std::cout << '\t' << command.Name()
					  << "\t\t" << command.Help() << '\n';

			++iterator;
		}
	});
}

void Application::addCommand(const char* name,
							 const char* description,
							 Function function)
{
	assert(name != nullptr);
	assert(description != nullptr);

	commands.emplace_front(commandsGroup, name, description, function);
}

void Application::addCommand(const char* name,
							 const char* description,
							 Command& command)
{
	addCommand(name, description, [&](args::Subparser& parser)
	{
		command.execute(parser);
	});
}

void Application::run()
{
	const size_t BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];

	do
	{
		std::cout << "$ ";
		std::cin.getline(buffer, BUFFER_SIZE);
		invokeCommand(buffer);
	} while (!receivedExitCommand);
}

void Application::invokeCommand(char* commandLine)
{
	StringCutter cutter;
	std::vector<std::string> arguments = cutter.cutToWords(commandLine);

	try
	{
		parser.ParseArgs(arguments);
	}
	catch (std::runtime_error& e)
	{
		Logger::logError(e);
	}
}
