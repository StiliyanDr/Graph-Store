#ifndef __STRING_HEADER_INCLUDED__
#define __STRING_HEADER_INCLUDED__

class String
{
public:
	String();
	String(char symbol);
	String(const char* string);
	String(String&& source);
	String(const String& source);
	String& operator=(const String &rhs);
	String& operator=(String &&rhs);
	~String();

	size_t getLength() const;
	const char* cString() const;

	void concatenate(const char* string);
	void concatenate(char symbol);

public:
	String& operator+=(const String& string);
	String& operator+=(const char* string);
	String& operator+=(char symbol);

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
String operator+(const String &lhs, char rhs);
String operator+(char lhs, const String &rhs);

#endif //__STRING_HEADER_INCLUDED__