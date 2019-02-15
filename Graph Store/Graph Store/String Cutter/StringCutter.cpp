#include "StringCutter.h"
#include <stdexcept>

StringCutter::StringCutter() noexcept :
	StringCutter(std::vector<char>())
{
}

StringCutter::StringCutter(std::vector<char> delimiters) :
	remainingString(nullptr),
	word(nullptr),
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

	char delimiter = skipAndReturnDelimiter();
	markWordStart();
	skipTo(delimiter);
	markWordEnd();

	std::string word = copySkippedWord();
	skipSpaces();

	return word;
}

void StringCutter::skipSpaces()
{
	while (!hasReachedEnd() && *remainingString == ' ')
	{
		++remainingString;
	}
}

char StringCutter::skipAndReturnDelimiter()
{
	char current = *remainingString;

	if (isDelimiter(current) && !hasReachedEnd())
	{
		++remainingString;
		return current;
	}
	else
	{
		return DEFAULT_DELIMITER;
	}
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

void StringCutter::markWordStart()
{
	word = remainingString;
}

void StringCutter::skipTo(char startDelimiter)
{
	while (!(hasReachedEnd() || *remainingString == startDelimiter))
	{
		++remainingString;
	}

	char endDelimiter =
		!hasReachedEnd() ? *remainingString : DEFAULT_DELIMITER;

	verifyDelimitersMatch(startDelimiter, endDelimiter);
}

void StringCutter::verifyDelimitersMatch(char lhs, char rhs)
{
	if (lhs != rhs)
	{
		throw std::logic_error("Word delimiters don't match!");
	}
}

void StringCutter::markWordEnd()
{
	if (hasReachedEnd())
	{
		return;
	}

	*remainingString = '\0';
	++remainingString;
}

std::string StringCutter::copySkippedWord()
{
	return std::string(word);
}
