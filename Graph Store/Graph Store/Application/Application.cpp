#include "Application.h"
#include "Logger\Logger.h"
#include "Command\Graph Command\GraphCommand.h"
#include "Runtime Error\RuntimeError.h"
#include "Graph IO\Directory Loader\DirectoryLoader.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

const String Application::COMMAND_PROMPT = "$ ";

Application& Application::instance()
{
	static Application theInstance;
	
	return theInstance;
}

Application::Application() :
	parser("GRAPH STORE APPLICATION"),
	commandsGroup(parser, "SUPPORTED COMMANDS"),
	cutter(Delimiters({ '\'', '\"' })),
	receivedExitCommand(false)
{
	addExitCommand();
	addHelpCommand();
}

void Application::addExitCommand()
{
	addCommand("exit",
		       "Terminates the program",
		       [&](args::Subparser& parser)
	{
		parser.Parse();
		receivedExitCommand = true;
	});
}

void Application::addHelpCommand()
{
	addCommand("help", "Lists the supported commands",
		       [&](args::Subparser& parser)
	{
		parser.Parse();
		
		std::cout << "Supported commands:\n";

		std::for_each(commands.cbegin(),
			          commands.cend(),
			          [](const args::Command& c)
		{
			std::cout << '\t' << c.Name()
					  << ": " << c.Help() << '\n';
		});
	});
}

void Application::addCommand(const char* name,
							 const char* description,
							 Function f)
{
	verifyStringIsNotNull(name);
	verifyStringIsNotNull(description);

	commands.emplace_front(commandsGroup, name, description, f);
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

void Application::runIn(const String& directory)
{
	try
	{
		setCurrentDirectory(directory);
		loadGraphsFrom(directory);
	}
	catch (std::exception& e)
	{
		Logger::logError(e);
		return;
	}

	passLoadedGraphsToGraphCommands();
	interact();
}

void Application::setCurrentDirectory(const String& path)
{
	try
	{
		std::filesystem::current_path(path.cString());
	}
	catch (std::filesystem::filesystem_error&)
	{
		throw RuntimeError("Could not change the directory to: " + path);
	}
}

void Application::loadGraphsFrom(const String& directory)
{
	GraphIO::DirectoryLoader loader;

	graphs = loader.load(directory);
}

void Application::passLoadedGraphsToGraphCommands()
{
	GraphCommand::setGraphs(graphs);
}

void Application::interact()
{
	const std::size_t COMMAND_LINE_SIZE = 512;
	char commandLine[COMMAND_LINE_SIZE];

	do
	{
		std::cout << COMMAND_PROMPT;
		std::cin.getline(commandLine, COMMAND_LINE_SIZE);
		invokeCommand(commandLine);
	} while (!receivedExitCommand);
}

void Application::invokeCommand(char* commandLine)
{
	assert(commandLine != nullptr);

	try
	{
		ArgumentsList arguments =
			cutter.cutToWords(commandLine);

		parser.ParseArgs(arguments);
	}
	catch (std::exception& e)
	{
		Logger::logError(e);
	}
}
