#ifndef __SEARCH_COMMAND_HEADER_INCLUDED__
#define __SEARCH_COMMAND_HEADER_INCLUDED__

#include "../Abstract class/Command.h"
#include "../String Reader/StringReader.h"

class SearchCommand : public Command
{
public:
	SearchCommand() = default;
	SearchCommand(const SearchCommand&) = delete;
	SearchCommand& operator=(const SearchCommand&) = delete;
	SearchCommand(SearchCommand&&) = delete;
	SearchCommand& operator=(SearchCommand&&) = delete;
	virtual ~SearchCommand() = default;
	
	virtual void execute(args::Subparser& parser) override;

private:
	static ShortestPathAlgorithm::Path findShortestPath(const String& sourceID,
														const String& targetID,
														const String& algorithmID);

private:
	void parseArguments(args::Subparser& parser);
	void setSourceID(args::Positional<String, StringReader>& id);
	void setTargetID(args::Positional<String, StringReader>& id);
	void setAlgorithmID(args::Positional<String, StringReader>& id);

private:
	String sourceID;
	String targetID;
	String algorithmID;
};

#endif //__SEARCH_COMMAND_HEADER_INCLUDED__