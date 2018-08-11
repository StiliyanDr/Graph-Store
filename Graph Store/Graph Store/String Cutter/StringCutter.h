#ifndef __STRING_CUTTER_HEADER_INCLUDED__
#define __STRING_CUTTER_HEADER_INCLUDED__

#include <string>
#include <vector>

class StringCutter
{
public:
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

private:
	char* remainingString;
};

#endif //__STRING_CUTTER_HEADER_INCLUDED__