#include "String.h"
#include <cstring>
#include <assert.h>

std::ostream& operator<<(std::ostream& out,
	                     const String& s)
{
	out << s.cString();

	return out;
}

bool operator!=(const String& lhs,
	            const String& rhs) noexcept
{
	return !(lhs == rhs);
}

bool operator==(const String& lhs,
	            const String& rhs) noexcept
{
	return std::strcmp(lhs.cString(),
		               rhs.cString()) == 0;
}

bool operator>(const String& lhs,
	           const String& rhs) noexcept
{
	return rhs < lhs;
}

bool operator>=(const String& lhs,
	            const String& rhs) noexcept
{
	return !(lhs < rhs);
}

bool operator<=(const String& lhs,
	            const String& rhs) noexcept
{
	return !(rhs < lhs);
}

bool operator<(const String& lhs,
	           const String& rhs) noexcept
{
	return std::strcmp(lhs.cString(),
		               rhs.cString()) < 0;
}

String operator "" _s(const char* string, std::size_t)
{
	return String{ string };
}

String::String(const char* string) :
	String{}
{
	setString(string);
}

String::String() noexcept :
	string(nullptr)
{
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

String::String(char c) :
	String{}
{
	char s[] = { c, '\0' };
	setString(s);
}

String::String(const String& source) :
	String{ source.string }
{
}

String::String(String&& source) noexcept :
	string(source.string)
{
	source.string = nullptr;
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

String operator+(const String& lhs,
	             const String& rhs)
{
	auto newString = lhs;
	newString += rhs;

	return newString;
}

String& String::operator+=(const String& s)
{
	concatenate(s.cString());

	return *this;
}

void String::concatenate(const char* s)
{
	if (s == nullptr || isEmptyString(s))
	{
		return;
	}

	replaceStringWith(concatenate(cString(), s));
}

bool String::isEmptyString(const char* s) noexcept
{
	assert(s != nullptr);

	return *s == '\0';
}

char* String::concatenate(const char* lhs,
	                      const char* rhs)
{
	assert(lhs != nullptr);
	assert(rhs != nullptr);

	auto lhsLength = std::strlen(lhs);
	auto rhsLength = std::strlen(rhs);
	auto totalLength = lhsLength + rhsLength + 1;
	auto buffer = new char[totalLength];

	std::strncpy(buffer, lhs, lhsLength + 1);
	std::strncat(buffer, rhs, rhsLength);

	return buffer;
}

String& String::operator+=(const char* string)
{
	concatenate(string);

	return *this;
}

String& String::operator+=(char c)
{
	char s[] = { c, '\0' };
	concatenate(s);

	return *this;
}

std::size_t String::getLength() const noexcept
{
	return std::strlen(cString());
}

const char* String::cString() const noexcept
{
	return (string != nullptr) ? string : "";
}

String::operator const char* () const noexcept
{
	return cString();
}
