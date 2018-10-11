#include "String.h"
#include <cstring>

bool operator!=(const String &lhs, const String &rhs)
{
	return !(lhs == rhs);
}

bool operator==(const String &lhs, const String &rhs)
{
	return strcmp(lhs.cString(), rhs.cString()) == 0;
}

bool operator>(const String &lhs, const String &rhs)
{
	return rhs < lhs;
}

bool operator>=(const String &lhs, const String &rhs)
{
	return !(lhs < rhs);
}

bool operator<=(const String &lhs, const String &rhs)
{
	return !(rhs < lhs);
}

bool operator<(const String &lhs, const String &rhs)
{
	return strcmp(lhs.cString(), rhs.cString()) < 0;
}

String operator+(const String &lhs, const String &rhs)
{
	String newString(lhs);
	newString += rhs.cString();

	return newString;
}

String::String() :
	string(nullptr)
{
}

String::String(char symbol) :
	string(nullptr)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	setString(buffer);
}

String::String(const char* string) :
	string(nullptr)
{
	setString(string);
}

String::String(String&& source) :
	string(source.string)
{
	source.string = nullptr;
}

String::String(const String &source) :
	string(nullptr)
{
	setString(source.string);
}

String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		setString(rhs.string);
	}

	return *this;
}

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

String::~String()
{
	delete[] string;
}

void String::setString(const char* newString)
{
	if (newString != nullptr)
	{
		size_t bufferSize = strlen(newString) + 1;
		char* buffer = new char[bufferSize];
		strcpy_s(buffer, bufferSize, newString);

		delete[] string;
		string = buffer;
	}
	else
	{
		delete[] string;
		string = nullptr;
	}
}

String& String::operator+=(const String& string)
{
	concatenate(string.cString());

	return *this;
}

String& String::operator+=(const char* string)
{
	concatenate(string);

	return *this;
}

String& String::operator+=(char symbol)
{
	concatenate(symbol);

	return *this;
}

void String::concatenate(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	concatenate(buffer);
}

void String::concatenate(const char* stringToConcatenate)
{
	if (stringToConcatenate == nullptr)
	{
		return;
	}

	size_t stringToConcatenateLength = strlen(stringToConcatenate);

	if (stringToConcatenateLength > 0)
	{
		size_t currentLength = getLength();
		size_t newLength = currentLength + stringToConcatenateLength + 1;
		char* buffer = new char[newLength];

		strcpy_s(buffer, currentLength + 1, cString());
		strcat_s(buffer, newLength, stringToConcatenate);

		delete[] string;
		string = buffer;
	}
}

size_t String::getLength() const
{
	return strlen(cString());
}

const char* String::cString() const
{
	return (string != nullptr) ? string : "";
}
