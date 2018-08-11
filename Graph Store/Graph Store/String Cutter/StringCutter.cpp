#include "StringCutter.h"
#include <assert.h>

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
	assert(string != nullptr);

	remainingString = string;
}

bool StringCutter::hasReachedEnd() const
{
	return *remainingString == '\0';
}

std::string StringCutter::cutWord()
{
	skipSpaces();
	const char* word = remainingString;
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
