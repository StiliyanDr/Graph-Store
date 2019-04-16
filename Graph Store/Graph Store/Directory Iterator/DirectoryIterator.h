#ifndef __DIRECTORY_ITERATOR_HEADER_INCLUDED__
#define __DIRECTORY_ITERATOR_HEADER_INCLUDED__

#include <filesystem>
#include <functional>

class String;

class DirectoryIterator
{
	using Iterator = std::filesystem::directory_iterator;
	using Path = std::filesystem::path;

public:
	using ExtensionFilter = std::function<bool (const String&)>;

	class AlwaysTrue
	{
	public:
		bool operator()(const String&) const noexcept
		{
			return true;
		}
	};

public:
	DirectoryIterator() = default;
	explicit DirectoryIterator(const String& path,
		                       ExtensionFilter filter = AlwaysTrue());
	~DirectoryIterator() = default;

	DirectoryIterator& operator++();
	operator bool() const;
	bool operator!() const;
	String operator*() const;

	friend bool operator==(const DirectoryIterator& lhs,
		                   const DirectoryIterator& rhs);

private:
	static String pathToString(const Path& p);

private:
	void tryToStartIterationIn(const String& path);
	void skipNonpassingFiles();
	bool isValid() const;
	String currentExtension() const;

private:
	static const Iterator end;

private:
	ExtensionFilter filter;
	Iterator iterator;
};

bool operator!=(const DirectoryIterator& lhs, const DirectoryIterator& rhs);

#endif //__DIRECTORY_ITERATOR_HEADER_INCLUDED__