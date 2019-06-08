#ifndef __EXTENSION_FILTER_HEADER_INCLUDED__
#define __EXTENSION_FILTER_HEADER_INCLUDED__

#include "String/String.h"

class ExtensionFilter
{
public:
	explicit ExtensionFilter(String extension) noexcept :
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

#endif //__EXTENSION_FILTER_HEADER_INCLUDED__