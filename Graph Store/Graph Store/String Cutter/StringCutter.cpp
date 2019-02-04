#include "StringCutter.h"
#include <stdexcept>

StringCutter::StringCutter() noexcept :
	StringCutter(std::vector<char>())
{
}

StringCutter::StringCutter(std::vector<char> delimiters) :
	delimiters(std::move(delimiters))
{
}

std::vector<std::string> StringCutter::cutToWords(char* string)
{
	setString(string);

	std::vector<std::string> words;

	while (!hasReachedEnd())
	{
		words.push_back(cutWord());
	}

	return words;
}

void StringCutter::setString(char* string)
{
	if (string != nullptr)
	{
		remainingString = string;
	}
	else
	{
		throw std::invalid_argument("Expected a string!");
	}
}

bool StringCutter::hasReachedEnd() const
{
	return *remainingString == '\0';
}

std::string StringCutter::cutWord()
{
	skipSpaces();
	markWordStart();
	skipWord();
	separateSkippedWord();
	skipSpaces();

	word = skipFirstSymbolIfQuotation(word);

	return word;
}

void StringCutter::skipSpaces()
{
	while (!hasReachedEnd() && *remainingString == ' ')
	{
		++remainingString;
	}
}

void StringCutter::markWordStart()
{
	word = remainingString;
}

void StringCutter::skipWord()
{
	char delimiter = ' ';

	if (*remainingString == '\'')
	{
		delimiter = '\'';
		++remainingString;
	}

	while (!(hasReachedEnd() || *remainingString == delimiter))
	{
		++remainingString;
	}
}

void StringCutter::separateSkippedWord()
{
	if (hasReachedEnd())
	{
		return;
	}

	*remainingString = '\0';
	++remainingString;
}

const char* StringCutter::skipFirstSymbolIfQuotation(const char* word)
{
	return *word != '\'' ? word : word + 1;
}

bool StringCutter::isDelimiter(char c) const
{
	size_t size = delimiters.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (delimiters[i] == c)
		{
			return true;
		}
	}

	return false;
}
