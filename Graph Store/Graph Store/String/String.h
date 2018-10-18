#ifndef __STRING_HEADER_INCLUDED__
#define __STRING_HEADER_INCLUDED__

#include <iostream>

class String
{
public:
	String();
	String(char character);
	String(const char* string);
	String(String&& source);
	String(const String& source);
	String& operator=(const String &rhs);
	String& operator=(String &&rhs);
	~String();

	size_t getLength() const;
	const char* cString() const;

	void concatenate(const char* string);
	void concatenate(char character);

public:
	String& operator+=(const String& string);
	String& operator+=(const char* string);
	String& operator+=(char character);

private:
	void setString(const char* newString);

private:
	char* string;
};

bool operator==(const String &lhs, const String &rhs);
bool operator!=(const String &lhs, const String &rhs);
bool operator<(const String &lhs, const String &rhs);
bool operator>(const String &lhs, const String &rhs);
bool operator>=(const String &lhs, const String &rhs);
bool operator<=(const String &lhs, const String &rhs);

String operator+(const String &lhs, const String &rhs);
String operator "" _s(const char* string, size_t);

std::ostream& operator<<(std::ostream& out, const String& s);

#endif //__STRING_HEADER_INCLUDED__