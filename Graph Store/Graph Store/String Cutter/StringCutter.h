#ifndef __STRING_CUTTER_HEADER_INCLUDED__
#define __STRING_CUTTER_HEADER_INCLUDED__

#include <string>
#include <vector>
#include <initializer_list>

class StringCutter
{
    class WordEndMarker
    {
    public:
        explicit WordEndMarker(char* end) noexcept :
            end(end),
            c(*end)
        {
            *end = '\0';
        }

        WordEndMarker(const WordEndMarker&) = delete;
        WordEndMarker& operator=(const WordEndMarker&) = delete;

        ~WordEndMarker()
        {
            *end = c;
        }

    private:
        char* end;
        char c;
    };

public:
	StringCutter() = default;
	explicit StringCutter(std::initializer_list<char> delimiters);

	std::vector<std::string> cutToWords(char* string);

private:
	static void verifyDelimitersMatch(char lhs, char rhs);

private:
	std::string cutWord();
	void skipSpaces() noexcept;
	char skipAndReturnDelimiter();
    void markWordStart() noexcept;
	void skipTo(char startDelimiter);
    std::string copySkippedWord();
    std::string temporarilyMarkWordEndAndMakeACopy();
	bool hasReachedEnd() const noexcept;
	void setString(char* s);
	bool isDelimiter(char c) const;

private:
	static const char DEFAULT_DELIMITER = ' ';

private:
	char* remainingString;
	const char* word;
	std::vector<char> delimiters;
};

#endif //__STRING_CUTTER_HEADER_INCLUDED__