#ifndef __DIRECTORY_LOADER_HEADER_INCLUDED__
#define __DIRECTORY_LOADER_HEADER_INCLUDED__

#include "../Graph Builder/GraphBuilder.h"

class GraphCollection;

namespace GraphIO
{
	class DirectoryLoader
	{
	public:
		GraphCollection load(const String& path);

	private:
		std::unique_ptr<Graph> loadFile(const String& pathName);

	private:
		GraphBuilder graphBuilder;
	};
}

#endif //__DIRECTORY_LOADER_HEADER_INCLUDED__