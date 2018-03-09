#ifndef __STRING_HEADER_INCLUDED__
#define __STRING_HEADER_INCLUDED__

class String
{
public:
	String(char symbol);
	String(const char* string = "");
	String(String&& src);
	String(const String& src);
	String& operator=(const String &rhs);
	String& operator=(String &&rhs);
	~String();

	size_t getLength() const;

	void concatenate(const char* string);
	void concatenate(char symbol);

public:
	String& operator+=(const char* rhs);
	String& operator+=(char rhs);
	operator const char*() const;

private:
	char* string;

private:
	const char* getString() const;
	void setString(const char* with);
};

bool operator==(const String &lhs, const String &rhs);
bool operator!=(const String &lhs, const String &rhs);
bool operator<(const String &lhs, const String &rhs);
bool operator>(const String &lhs, const String &rhs);
bool operator>=(const String &lhs, const String &rhs);
bool operator<=(const String &lhs, const String &rhs);

String operator+(const String &lhs, const String &rhs);
String operator+(const char* lhs, const String &rhs);
String operator+(const String &lhs, const char* rhs);
String operator+(const String &lhs, char rhs);
String operator+(char lhs, const String &rhs);

#endif //__STRING_HEADER_INCLUDED__