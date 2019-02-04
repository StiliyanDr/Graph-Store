#ifndef __STRING_CUTTER_HEADER_INCLUDED__
#define __STRING_CUTTER_HEADER_INCLUDED__

#include <string>
#include <vector>

class StringCutter
{
public:
	StringCutter() noexcept;
	explicit StringCutter(std::vector<char> delimiters);

	std::vector<std::string> cutToWords(char* string);

private:
	static const char* skipFirstSymbolIfQuotation(const char* word);

private:
	std::string cutWord();
	void skipSpaces();
	void skipWord();
	void separateSkippedWord();
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