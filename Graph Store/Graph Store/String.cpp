#include "stdafx.h"
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


bool operator<(const String &lhs, const String &rhs)
{
	return strcmp(lhs, rhs) < 0;
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


String operator+(const String &lhs, const String &rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


String operator+(const String &lhs, char rhs)
{
	String tmp(lhs);
	tmp += rhs;

	return tmp;
}


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


String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		String temp(rhs);
		std::swap(string, temp.string);
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
