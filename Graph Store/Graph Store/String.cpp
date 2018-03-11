#include "String.h"
#include <cstring>
#include <utility>


bool operator==(const String &lhs, const String &rhs)
{
	return !strcmp(lhs, rhs);
}


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
///Moves source's string into the current object.
///
String::String(String&& source) :
	string(source.string)
{
	source.string = nullptr;
}


///
///Make a copy of rhs, swap contents with the copy and
///let it go out of scope, deleting the old string.
///
String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		String temp(rhs);
		std::swap(string, temp.string);
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
		delete[] string;		
		string = rhs.string;	
		rhs.string = nullptr;	
	}
	return *this;
}


///
///Makes a copy of source's string
///and stores it in the object.
///
String::String(const String &source) :
	string(nullptr)
{
	setString(source.string);
}


String::~String()
{
	delete[] string;
}


size_t String::getLength() const
{
	return strlen(getString());
}


///
///Concatenates a copy of the passed string
///to the string stored in the object.
///
void String::concatenate(const char* argument)
{
	if (argument)
	{
		size_t argLen = strlen(argument);

		if (argLen)
		{
			size_t currLen = getLength();
			size_t newLen = currLen + argLen + 1;
			char* buffer = new char[newLen];

			strcpy_s(buffer, currLen + 1, getString());
			strcat_s(buffer, newLen, argument);

			delete[] string;
			string = buffer;
		}
	}
}


void String::concatenate(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	concatenate(buffer);
}


String& String::operator+=(const char* rhs)
{
	concatenate(rhs);

	return *this;
}


String& String::operator+=(char rhs)
{
	concatenate(rhs);

	return *this;
}


String::operator const char *() const
{
	return getString();
}


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
void String::setString(const char* with)
{
	if (with)
	{
		size_t buffSize = strlen(with) + 1;
		char* buffer = new char[buffSize];
		strcpy_s(buffer, buffSize, with);

		delete[] string;
		string = buffer;
	}
	else
	{
		delete[] string;
		string = nullptr;
	}
}
