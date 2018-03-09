#include "stdafx.h"
#include "String.h"
#include <cstring>
#include <utility>


///
///Checks the two strings for equality.
///
bool operator==(const String &lhs, const String &rhs)
{
	return !strcmp(lhs, rhs);
}


///
///Checks the two strings for inequality.
///
bool operator!=(const String &lhs, const String &rhs)
{
	return !(lhs == rhs);
}


///
///Checks whether lhs is lexicographically smaller than rhs.
///
bool operator<(const String &lhs, const String &rhs)
{
	return strcmp(lhs, rhs) < 0;
}


///
///Checks whether lhs is lexicographically greater than rhs.
///
bool operator>(const String &lhs, const String &rhs)
{
	return rhs < lhs;
}


///
///Checks whether lhs is lexicographically
///greater or equal to rhs.
///
bool operator>=(const String &lhs, const String &rhs)
{
	return !(lhs < rhs);
}


///
///Checks whether lhs is lexicographically
///smaller or equal to rhs.
///
bool operator<=(const String &lhs, const String &rhs)
{
	return !(rhs < lhs);
}


///
///Returns the string concatenation of lhs and rhs.
///
String operator+(const String &lhs, const String &rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


///
///Returns the string concatenation of lhs and rhs.
///
String operator+(const char* lhs, const String &rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


///
///Returns the string concatenation of lhs and rhs.
///
String operator+(const String &lhs, const char* rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


///
///Returns the string concatenation of lhs and rhs.
///
String operator+(const String &lhs, char rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


///
///Returns the string concatenation of lhs and rhs.
///
String operator+(char lhs, const String &rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


String::String(char symbol) :
	string(nullptr)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	setString(buffer);
}


///
///Makes a copy of the passed string
///and stores it in the object.
///
String::String(const char* string) :
	string(nullptr)
{
	setString(string);
}


///
///Moves src's string into the current object.
///
String::String(String&& src) :
	string(src.string)
{
	src.string = nullptr;
}


///
/// Using the copy-and-swap idiom to implement
/// operator= for arguments of type:
///
/// 1) const char*: The parameter is initialised with the String(const char*) constructor. 
/// 2) rvalue String: the move constructor initialises the parameter.
/// 3) lvalue String: the copy constructor initialises the parameter.
///
/// All these cases are handled with this implementation.
///
/// String& String::operator=(String rhs)
/// {
///		std::swap(this->string, rhs.string);
///
///		return *this;
/// }
///


///
///Make a copy of rhs, swap contents with the copy and
///let it go out of scope, deleting the old string.
///
String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		String tmp(rhs);
		std::swap(string, tmp.string);
	}
	return *this;
}


///
///Move rhs into the current object and leave it
///with a null pointer (rhs becomes the empty string).
///
String& String::operator=(String&& rhs)
{
	if (this != &rhs)
	{
		delete[] string;		//Release the old string
		string = rhs.string;	//and move rhs' string into 
		rhs.string = nullptr;	//the current object.
	}
	return *this;
}


///
///Makes a copy of src's string
///and stores it in the object.
///
String::String(const String &src) :
	string(nullptr)
{
	setString(src.string);
}


///
///Release memory.
///
String::~String()
{
	delete[] string;
}


///
///Returns the length of the string
///that is stored in the object.
///
size_t String::getLength() const
{
	return strlen(getString());
}


///
///Concatenates a copy of the passed string
///to the string stored in the object.
///
void String::concatenate(const char* str)
{
	if (str)
	{
		size_t newLen, currLen, argLen = strlen(str);

		if (argLen)
		{
			currLen = getLength();
			newLen = currLen + argLen + 1;
			char* buffer = new char[newLen];

			strcpy_s(buffer, currLen + 1, getString());
			strcat_s(buffer, newLen, str);

			delete[] string;
			string = buffer;
		}
	}
}


///
///Concatenates symbol at the end of the string.
///
void String::concatenate(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	concatenate(buffer);
}


///
///Concatenates rhs to the string.
///
String& String::operator+=(const char* rhs)
{
	concatenate(rhs);

	return *this;
}


///
///Concatenates rhs to the string.
///
String& String::operator+=(char rhs)
{
	concatenate(rhs);

	return *this;
}


String::operator const char *() const
{
	return getString();
}


///
///Returns the string stored in the object
///or the empty string if the member is nullptr.
///
const char* String::getString() const
{
	return (string) ? string : "";
}


///
///Makes a copy of the argument and stores
///it in the object, releasing the old string 
///first(if any).
///
///If nullptr is passed, the member becomes
///nullptr and the object represents the empty 
///string.
///
void String::setString(const char* value)
{
	if (value)
	{
		size_t buffSize = strlen(value) + 1;
		char* buffer = new char[buffSize];
		strcpy_s(buffer, buffSize, value);

		delete[] string;
		string = buffer;
	}
	else
	{
		delete[] string;
		string = nullptr;
	}
}
