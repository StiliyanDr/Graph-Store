#pragma once
#include "String\String.h"

class ExtensionFilter
{
public:
	explicit ExtensionFilter(String extension) :
		extension(std::move(extension))
	{
	}

	bool operator()(const String& extension) const
	{
		return extension == this->extension;
	}

private:
	String extension;
};
