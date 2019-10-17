#include "StringCutter.h"
#include <stdexcept>

StringCutter::StringCutter(
    std::initializer_list<char> delimiters
) :
	remainingString(nullptr),
	word(nullptr),
	delimiters(delimiters)
{
}

std::vector<std::string>
StringCutter::cutToWords(char* string)
{
    setString(string);

    auto words = std::vector<std::string>{};

	while (!hasReachedEnd())
	{
		words.push_back(cutWord());
	}

	return words;
}

void StringCutter::setString(char* s)
{
	if (s != nullptr)
	{
		remainingString = s;
	}
	else
	{
        throw std::invalid_argument{ "Expected a string!" };
	}
}

bool StringCutter::hasReachedEnd() const noexcept
{
	return *remainingString == '\0';
}

std::string StringCutter::cutWord()
{
    skipSpaces();

	auto delimiter = skipAndReturnDelimiter();
	markWordStart();
	skipTo(delimiter);

	auto word = copySkippedWord();
	skipSpaces();

	return word;
}

void StringCutter::skipSpaces() noexcept
{
	while (!hasReachedEnd() && *remainingString == ' ')
	{
		++remainingString;
	}
}

char StringCutter::skipAndReturnDelimiter()
{
	auto c = *remainingString;

	if (isDelimiter(c) && !hasReachedEnd())
	{
		++remainingString;
		return c;
	}
	else
	{
		return DEFAULT_DELIMITER;
	}
}

bool StringCutter::isDelimiter(char c) const
{
    auto end = std::end(delimiters);

    return std::find(std::begin(delimiters),
                     end,
                     c) != end;
}

void StringCutter::markWordStart() noexcept
{
	word = remainingString;
}

void StringCutter::skipTo(char startDelimiter)
{
    while (!(hasReachedEnd() ||
             *remainingString == startDelimiter))
    {
        ++remainingString;
    }

    auto endDelimiter =
        !hasReachedEnd() ? *remainingString : DEFAULT_DELIMITER;

    verifyDelimitersMatch(startDelimiter, endDelimiter);
}

void StringCutter::verifyDelimitersMatch(char lhs, char rhs)
{
    if (lhs != rhs)
    {
        throw std::logic_error{ "Word delimiters don't match!" };
    }
}

std::string StringCutter::copySkippedWord()
{
    auto delimiter = *remainingString;
    *remainingString = '\0';

    auto result = std::string{ word };

    *remainingString = delimiter;

    if (!hasReachedEnd())
    {
        ++remainingString;
    }

    return result;
}
