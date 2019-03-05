#include "DirectoryIterator.h"
#include "DirectoryIteratorException.h"
#include "String\String.h"
#include <assert.h>
#include <stdexcept>

const DirectoryIterator::Iterator DirectoryIterator::end;

DirectoryIterator::DirectoryIterator(const String& path,
	                                 ExtensionFilter f) :
	filter(std::move(f))
{
	tryToStartIterationIn(path);
	skipNonpassingFiles();
}

void DirectoryIterator::tryToStartIterationIn(const String& path)
{
	assert(!isValid());

	try
	{
		iterator = Iterator(Path(path.cString()));
	}
	catch (std::experimental::filesystem::filesystem_error& e)
	{
		throw DirectoryIteratorException(String(e.what()));
	}
}

void DirectoryIterator::skipNonpassingFiles()
{
	while (isValid() && !filter(currentExtension()))
	{
		++iterator;
	}
}

String DirectoryIterator::currentExtension() const
{
	assert(isValid());

	const Path& p = iterator->path();
	
	return pathToString(p.extension());
}

String DirectoryIterator::pathToString(const Path& p)
{
	std::string s = p.string();

	return String(s.c_str());
}

String DirectoryIterator::operator*() const
{
	if (isValid())
	{
		return pathToString(iterator->path());
	}
	else
	{
		throw std::out_of_range("Iterator out of range!");
	}
}

DirectoryIterator& DirectoryIterator::operator++()
{
	if (isValid())
	{
		++iterator;
		skipNonpassingFiles();
	}

	return *this;
}

DirectoryIterator::operator bool() const
{
	return isValid();
}

bool DirectoryIterator::operator!() const
{
	return !isValid();
}

bool DirectoryIterator::isValid() const
{
	return iterator != end;
}

bool operator!=(const DirectoryIterator& lhs,
	            const DirectoryIterator& rhs)
{
	return !(lhs == rhs);
}

bool operator==(const DirectoryIterator& lhs,
	            const DirectoryIterator& rhs)
{
	return lhs.iterator == rhs.iterator;
}
