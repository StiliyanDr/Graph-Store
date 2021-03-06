#ifndef __STRING_CUTTER_HEADER_INCLUDED__
#define __STRING_CUTTER_HEADER_INCLUDED__

#include <string>
#include <vector>
#include <initializer_list>

class StringCutter
{
public:
	StringCutter() = default;
	explicit StringCutter(std::initializer_list<char> delimiters);

	std::vector<std::string> cutToWords(char* string);

private:
	static void verifyDelimitersMatch(char lhs, char rhs);

private:
	std::string cutWord();
	void skipSpaces();
	char skipAndReturnDelimiter();
	void skipTo(char startDelimiter);
	std::string copySkippedWord();
	void markWordEnd();
	bool hasReachedEnd() const;
	void setString(char* string);
	void markWordStart();
	bool isDelimiter(char c) const;

private:
	static const char DEFAULT_DELIMITER = ' ';

private:
	char* remainingString;
	const char* word;
	std::vector<char> delimiters;
};

#endif //__STRING_CUTTER_HEADER_INCLUDED__