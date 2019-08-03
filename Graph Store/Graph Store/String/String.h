#ifndef __STRING_HEADER_INCLUDED__
#define __STRING_HEADER_INCLUDED__

#include <ostream>

class String
{
public:
	String() noexcept;
	String(char c);
	String(const char* string);
	String(String&& source) noexcept;
	String& operator=(String&& rhs) noexcept;
	String(const String& source);
	String& operator=(const String& rhs);
	~String();

	std::size_t getLength() const noexcept;
	const char* cString() const noexcept;

public:
	String& operator+=(const String& string);
	String& operator+=(const char* string);
	String& operator+=(char c);
	explicit operator const char*() const noexcept;

private:
	static char* makeACopyOf(const char* string);

private:
	void concatenate(const char* string);
	void concatenate(char c);
	void setString(const char* newString);
	void replaceStringWith(char* newString) noexcept;
	void swapContentsWith(String s) noexcept;

private:
	char* string;
};

bool operator==(const String& lhs, const String& rhs) noexcept;
bool operator!=(const String& lhs, const String& rhs) noexcept;
bool operator<(const String& lhs, const String& rhs) noexcept;
bool operator>(const String& lhs, const String& rhs) noexcept;
bool operator>=(const String& lhs, const String& rhs) noexcept;
bool operator<=(const String& lhs, const String& rhs) noexcept;

String operator+(const String& lhs, const String& rhs);
String operator "" _s(const char* string, std::size_t);

std::ostream& operator<<(std::ostream& out, const String& s);

#endif //__STRING_HEADER_INCLUDED__