#include "String.h"
#include <cstring>

std::ostream& operator<<(std::ostream& out, const String& s)
{
	out << s.cString();

	return out;
}

bool operator!=(const String& lhs, const String& rhs) noexcept
{
	return !(lhs == rhs);
}

bool operator==(const String& lhs, const String& rhs) noexcept
{
	return std::strcmp(lhs.cString(), rhs.cString()) == 0;
}

bool operator>(const String& lhs, const String& rhs) noexcept
{
	return rhs < lhs;
}

bool operator>=(const String& lhs, const String& rhs) noexcept
{
	return !(lhs < rhs);
}

bool operator<=(const String& lhs, const String& rhs) noexcept
{
	return !(rhs < lhs);
}

bool operator<(const String& lhs, const String& rhs) noexcept
{
	return std::strcmp(lhs.cString(), rhs.cString()) < 0;
}

String operator "" _s(const char* string, std::size_t)
{
	return String{ string };
}

String::String() noexcept :
	string(nullptr)
{
}

String::String(char c) :
	String{}
{
	char s[] = { c, '\0' };
	setString(s);
}

void String::setString(const char* newString)
{
	replaceStringWith(makeACopyOf(newString));
}

char* String::makeACopyOf(const char* string)
{
	if (string == nullptr)
	{
		return nullptr;
	}

	auto bufferSize = std::strlen(string) + 1;
	auto buffer = new char[bufferSize];
	std::strncpy(buffer, string, bufferSize);

	return buffer;
}

void String::replaceStringWith(char* newString) noexcept
{
	delete[] string;
	string = newString;
}

String::String(const char* string) :
	String{}
{
	setString(string);
}

String::String(String&& source) noexcept :
	string(source.string)
{
	source.string = nullptr;
}

String::String(const String& source) :
	String{ source.string }
{
}

String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(rhs);
	}

	return *this;
}

void String::swapContentsWith(String s) noexcept
{
	std::swap(string, s.string);
}

String& String::operator=(String&& rhs) noexcept
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

String::~String()
{
	delete[] string;
}

String operator+(const String &lhs, const String &rhs)
{
	String newString(lhs);
	newString += rhs;

	return newString;
}

String& String::operator+=(const String& string)
{
	concatenate(string.cString());

	return *this;
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

String& String::operator+=(const char* string)
{
	concatenate(string);

	return *this;
}

String& String::operator+=(char character)
{
	concatenate(character);

	return *this;
}

void String::concatenate(char character)
{
	char buffer[2] = "";
	buffer[0] = character;

	concatenate(buffer);
}

size_t String::getLength() const noexcept
{
	return strlen(cString());
}

const char* String::cString() const noexcept
{
	return (string != nullptr) ? string : "";
}
