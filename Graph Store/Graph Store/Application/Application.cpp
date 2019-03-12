#include "Application.h"
#include <stdexcept>
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

		std::for_each(commands.cbegin(), commands.cend(), [&](const args::Command& command)
		{
			std::cout << '\t' << command.Name()
					  << ": " << command.Help() << '\n';
		});
	});
}

void Application::addCommand(const char* name,
							 const char* description,
							 Function function)
{
	verifyStringIsNotNull(name);
	verifyStringIsNotNull(description);

	commands.emplace_front(commandsGroup, name, description, function);
}

void Application::verifyStringIsNotNull(const char* s)
{
	if (s == nullptr)
	{
		throw std::invalid_argument("Expected a string!");
	}
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
	StringCutter cutter({ '\'', '\"' });
	std::vector<std::string> arguments = cutter.cutToWords(commandLine);

	try
	{
		parser.ParseArgs(arguments);
	}
	catch (std::exception& e)
	{
		Logger::logError(e);
	}
}
